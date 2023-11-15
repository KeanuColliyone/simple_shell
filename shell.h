#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFFER_SIZE 1024

struct Alias {
    char name[20];
    char value[50];
};

extern struct Alias aliases[10];
extern int num_aliases;

void execute_command_with_logical_operators(char *args[], int is_interactive);
void execute_command_with_logical_operators_and_alias(char *args[], int is_interactive);
void execute_commands_from_file(const char *filename);
void process_commands_from_file(const char *filename, int is_interactive);
void process_command(char *command, int is_interactive);
void execute_command(char *args[]);
void execute_command_with_alias(char *args[]);
void set_environment_variable(char *args[]);
void unset_environment_variable(char *args[]);
void change_directory(char *args[]);
void handle_alias(char *args[]);

#endif

