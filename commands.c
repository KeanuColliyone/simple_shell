#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "shell.h"

#define MAX_ALIASES 10
#define MAX_ALIAS_NAME 20
#define MAX_ALIAS_VALUE 50

int num_aliases = 0;

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

void execute_command_with_alias(char *args[]) {

	int i;

    for (i = 0; i < num_aliases; ++i) {
        if (strcmp(args[0], aliases[i].name) == 0) {
            args[0] = aliases[i].value;
            break;
        }
    }

    execute_command(args);
}

void set_environment_variable(char *args[]) {
    if (args[1] != NULL && args[2] != NULL) {
        if (setenv(args[1], args[2], 1) != 0) {
            perror("setenv");
        }
    } else {
        fprintf(stderr, "Usage: setenv VARIABLE VALUE\n");
    }
}

void unset_environment_variable(char *args[]) {
    if (args[1] != NULL) {
        if (unsetenv(args[1]) != 0) {
            perror("unsetenv");
        }
    } else {
        fprintf(stderr, "Usage: unsetenv VARIABLE\n");
    }
}

void change_directory(char *args[]) {
    char *new_dir = args[1];

    if (new_dir == NULL) {
        new_dir = getenv("HOME");
    }

    if (chdir(new_dir) != 0) {
        perror("cd");
    } else {
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            setenv("PWD", cwd, 1);
        } else {
            perror("getcwd");
        }
    }
}

void handle_alias(char *args[]) {

	int i;

    if (args[1] != NULL && args[2] != NULL) {
        if (num_aliases < MAX_ALIASES) {
            strncpy(aliases[num_aliases].name, args[1], MAX_ALIAS_NAME - 1);
            strncpy(aliases[num_aliases].value, args[2], MAX_ALIAS_VALUE - 1);
            num_aliases++;
        } else {
            fprintf(stderr, "Max number of aliases reached\n");
        }
    } else if (args[1] != NULL) {
        for (i = 0; i < num_aliases; ++i) {
            if (strcmp(args[1], aliases[i].name) == 0) {
                printf("%s='%s'\n", aliases[i].name, aliases[i].value);
                return;
            }
        }
        fprintf(stderr, "Alias not found: %s\n", args[1]);
    } else {
        for (i = 0; i < num_aliases; ++i) {
            printf("%s='%s'\n", aliases[i].name, aliases[i].value);
        }
    }
}

