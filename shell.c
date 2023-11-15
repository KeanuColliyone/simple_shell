#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "shell.h"

#define BUFFER_SIZE 1024

void custom_getline(char *buffer, size_t size) {
    if (fgets(buffer, size, stdin) == NULL) {
        if (feof(stdin)) {
            printf("EOF\n");
            exit(EXIT_SUCCESS);
        } else {
            perror("read");
            exit(EXIT_FAILURE);
        }
    }
}

int main(int argc, char *argv[]) {
    char buffer[BUFFER_SIZE];

    if (argc == 2) {
        FILE *file = fopen(argv[1], "r");
        if (file == NULL) {
            perror("Error opening file");
            return EXIT_FAILURE;
        }

        while (fgets(buffer, sizeof(buffer), file) != NULL) {
            size_t n = strlen(buffer);
            if (n > 0 && buffer[n - 1] == '\n') {
                buffer[n - 1] = '\0';
            }

            size_t j;
            int found;
            char *path;
            char *path_copy;
            char *dir;
            char cmd_path[BUFFER_SIZE];
            size_t I;
            size_t arg_start;

            char *args[BUFFER_SIZE];

            I = 0;
            arg_start = 0;
            for (j = 0; j <= n; ++j) {
                if (buffer[j] == ' ' || buffer[j] == '\0') {
                    args[I++] = &buffer[arg_start];
                    buffer[j] = '\0';
                    arg_start = j + 1;
                }
            }
            args[I] = NULL;

            if (strcmp(args[0], "exit") == 0) {
                fclose(file);
                return EXIT_SUCCESS;
            } else if (strcmp(args[0], "env") == 0) {
                print_environment();
            } else if (strcmp(args[0], "setenv") == 0) {
                set_environment_variable(args);
            } else if (strcmp(args[0], "unsetenv") == 0) {
                unset_environment_variable(args);
            } else if (strcmp(args[0], "cd") == 0) {
                change_directory(args);
            } else if (strcmp(args[0], "alias") == 0) {
                handle_alias(args);
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
        }

        fclose(file);
        return EXIT_SUCCESS;
    }

    while (1) {
        printf("$ ");
        custom_getline(buffer, BUFFER_SIZE);

        size_t n = strlen(buffer);
        if (n > 0 && buffer[n - 1] == '\n') {
            buffer[n - 1] = '\0';
        }

        size_t I;
        size_t arg_start;

        char *args[BUFFER_SIZE];

        I = 0;
        arg_start = 0;
        for (size_t j = 0; j <= n; ++j) {
            if (buffer[j] == ' ' || buffer[j] == '\0') {
                args[I++] = &buffer[arg_start];
                buffer[j] = '\0';
                arg_start = j + 1;
            }
        }
        args[I] = NULL;

        if (strcmp(args[0], "exit") == 0) {
            printf("Exiting the shell\n");
            break;
        } else if (strcmp(args[0], "env") == 0) {
            print_environment();
        } else {
            int found = 0;

            if (strchr(args[0], '/') != NULL) {
                if (access(args[0], X_OK) == 0) {
                    found = 1;
                    execute_command(args);
                }
            } else {
                char *path = getenv("PATH");
                char *path_copy = strdup(path);
                char *dir = strtok(path_copy, ":");

                while (dir != NULL) {
                    char cmd_path[BUFFER_SIZE];
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
    }

    return EXIT_SUCCESS;
}

