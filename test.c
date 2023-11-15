#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

extern char **environ;

#define BUFFER_SIZE 1024

void execute_command(char *args[]) {
    pid_t child_pid;
    int status;

    child_pid = fork();

    if (child_pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (child_pid == 0) {
        if (execvp(args[0], args) == -1) {
            perror(args[0]);
            exit(EXIT_FAILURE);
        }
    } else {
        waitpid(child_pid, &status, 0);
    }
}

void print_environment();

int main(void) {
    char buffer[BUFFER_SIZE];
    size_t n;
    char *token;
    char *args[BUFFER_SIZE];
    int i;
    int found;
    char *path;
    char *path_copy;
    char *dir;
    char cmd_path[BUFFER_SIZE];

    int is_interactive = isatty(STDIN_FILENO);

    while (1) {
        if (is_interactive) {
            printf("$ ");
        }
        
        if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
            printf("\n");
            break;
        }

        n = strlen(buffer);
        if (n > 0 && buffer[n - 1] == '\n') {
            buffer[n - 1] = '\0';
        }

        i = 0;
        token = strtok(buffer, " ");
        while (token != NULL) {
            args[i++] = token;
            token = strtok(NULL, " ");
        }
        args[i] = NULL;

        if (strcmp(args[0], "exit") == 0) {
            printf("Exiting the shell\n");
            break;
        } else if (strcmp(args[0], "env") == 0) {
            print_environment();
        } else {
            found = 0;

            if (strchr(args[0], '/') != NULL) {
                if (access(args[0], X_OK) == 0) {
                    found = 1;
                    execute_command(args);
                }
            } else {
                path = getenv("PATH");
                path_copy = strdup(path);
                dir = strtok(path_copy, ":");

                while (dir != NULL) {
                    snprintf(cmd_path, BUFFER_SIZE, "%s/%s", dir, args[0]);

                    if (access(cmd_path, X_OK) == 0) {
                        found = 1;
                        execute_command(args);
                        break;
                    }

                    dir = strtok(NULL, ":");
                }

                free(path_copy);
            }

            if (!found) {
                printf("Command not found: %s\n", args[0]);
            }
        }

        if (!is_interactive) {
            break;
        }
    }

    return EXIT_SUCCESS;
}

void print_environment() {
    char **env = environ;
    while (*env != NULL) {
        printf("%s\n", *env);
        env++;
    }
}

