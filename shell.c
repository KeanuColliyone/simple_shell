#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

extern char **environ;

#define BUFFER_SIZE 1024
#define MAX_ARGS 64

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
    char commands[MAX_ARGS][BUFFER_SIZE];
    size_t command_count = 0;
    size_t i;
    int start;
    char *args[MAX_ARGS];
    int arg_count;
    int start_arg;
    size_t j;

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

        command_count = 0;
        start = 0;
        for (i = 0; i <= n; i++) {
            if (buffer[i] == ';' || buffer[i] == '\0') {
                memcpy(commands[command_count], &buffer[start], i - start);
                commands[command_count][i - start] = '\0';
                command_count++;
                start = i + 1;
            }
        }

        for (i = 0; i < command_count; i++) {
            arg_count = 0;
            start_arg = 0;
            for (j = 0; j <= strlen(commands[i]); j++) {
                if (commands[i][j] == ' ' || commands[i][j] == '\0') {
		    args[arg_count] = &commands[i][start_arg];
                    arg_count++;
                    start_arg = j + 1;
                }
            }

	    args[arg_count] = NULL;

            if (strcmp(args[0], "exit") == 0) {
                printf("Exiting the shell\n");
                exit(EXIT_SUCCESS);
            } else if (strcmp(args[0], "env") == 0) {
                print_environment();
            } else {
                execute_command(args);
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

