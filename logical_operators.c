// logical_operators.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "shell.h"

void execute_command_with_logical_operators(char *args[], int is_interactive) {
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

        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            if (is_interactive) {
                printf("($)\n");
            }
        } else {
            if (is_interactive) {
                printf("(!)\n");
            }
        }
    }
}

void execute_command_with_logical_operators_and_alias(char *args[], int is_interactive) {
    // Check if the command is an alias
    for (int i = 0; i < num_aliases; ++i) {
        if (strcmp(args[0], aliases[i].name) == 0) {
            // Replace the alias with its value
            args[0] = aliases[i].value;
            break;
        }
    }

    execute_command_with_logical_operators(args, is_interactive);
}

