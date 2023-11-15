#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "shell.h"

#define BUFFER_SIZE 1024

void execute_commands_from_file(const char *filename) {

	char buffer[BUFFER_SIZE];
	size_t n;
	char *args[BUFFER_SIZE];
        size_t j;
	size_t I;
	size_t arg_start;
	int exit_status;

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror(filename);
        exit(EXIT_FAILURE);
    }

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        n = strlen(buffer);
        if (n > 0 && buffer[n - 1] == '\n') {
            buffer[n - 1] = '\0';
        }

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
            if (I > 1) {
                exit_status = atoi(args[1]);
                printf("Exiting the shell with status %d\n", exit_status);
                exit(exit_status);
            } else {
                printf("Exiting the shell\n");
                exit(EXIT_SUCCESS);
            }
        } else {
            execute_command_with_alias(args);
        }
    }

    fclose(file);
}

