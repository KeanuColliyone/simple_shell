#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "shell.h"

void process_command(char *command, int is_interactive) {
    size_t n = strlen(command);
    char *args[BUFFER_SIZE];
    size_t j;
    size_t I;
    size_t arg_start;
    size_t i;

    if (n > 0 && command[n - 1] == '\n') {
        command[n - 1] = '\0';
    }

    if (command[0] == '#' || command[0] == '\0') {
        return;
    }

    I = 0;
    arg_start = 0;

    for (j = 0; j <= n; ++j) {
        if (command[j] == ' ' || command[j] == '\0') {
            args[I++] = &command[arg_start];
            command[j] = '\0';
            arg_start = j + 1;
        }
    }
    args[I] = NULL;

    for (i = 0; i < I; ++i) {
        if (args[i][0] == '$') {
            char *env_value = getenv(args[i] + 1);
            if (env_value != NULL) {
                args[i] = env_value;
            } else {
                fprintf(stderr, "Variable not found: %s\n", args[i] + 1);
                return;
            }
        }
    }

    if (strcmp(args[0], "exit") == 0) {
        if (I > 1) {
            int exit_status = atoi(args[1]);
            printf("Exiting the shell with status %d\n", exit_status);
            exit(exit_status);
        } else {
            printf("Exiting the shell\n");
            exit(EXIT_SUCCESS);
        }
    } else {
        execute_command_with_logical_operators_and_alias(args, is_interactive);
    }
}

void process_commands_from_file(const char *filename, int is_interactive) {
    
	char buffer[BUFFER_SIZE];

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror(filename);
        exit(EXIT_FAILURE);
    }

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        process_command(buffer, is_interactive);
    }

    fclose(file);
}

