#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

extern char **environ;

#define BUFFER_SIZE 1024

char **tokenize_input(char *input) {

	int i;
	char *token;

    char **tokens = (char **)malloc(BUFFER_SIZE * sizeof(char *));
    if (tokens == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    i = 0;
    token = strtok(input, " \t\n");
    while (token != NULL) {
        tokens[i++] = token;
        token = strtok(NULL, " \t\n");
    }
    tokens[i] = NULL;

    return tokens;
}

char *custom_getline(void) {
    static char buffer[BUFFER_SIZE];
    static int buffer_index = 0;
    static int chars_read = 0;

    char *line = NULL;
    int index = 0;
    int newline_found = 0;
    int copy_length;
    int i;

    while (!newline_found) {
        if (buffer_index >= chars_read) {
            chars_read = read(STDIN_FILENO, buffer, BUFFER_SIZE);
            buffer_index = 0;

            if (chars_read <= 0) {
                if (line != NULL && index > 0) {
                    line[index] = '\0';
                    return line;
                }
                return NULL;
            }
        }

        while (buffer_index < chars_read) {
            if (buffer[buffer_index] == '\n') {
                newline_found = 1;
                break;
            }
            buffer_index++;
        }

        copy_length = buffer_index - index;
        if (copy_length > 0) {
            line = (char *)realloc(line, (index + copy_length + 1) * sizeof(char));

            for (i = 0; i < copy_length; i++) {
                line[index++] = buffer[i];
            }
        }

        if (newline_found) {
            buffer_index++;
            line[index++] = '\n';
            line[index] = '\0';
            return line;
        }
    }

    return NULL;
}

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
    char *buffer;
    char **args;
    size_t n;

    int is_interactive = isatty(STDIN_FILENO);

    while (1) {
        if (is_interactive) {
            printf("$ ");
        }
        
        if ((buffer = custom_getline()) == NULL) {
            printf("\n");
            break;
        }

        n = strlen(buffer);
        if (n > 0 && buffer[n - 1] == '\n') {
            buffer[n - 1] = '\0';
        }

	args = tokenize_input(buffer);

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

	free(buffer);
	free(args);
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

