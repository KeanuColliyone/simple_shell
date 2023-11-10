0x16. C - Simple Shell

UNIX command line interpreter.

Usage: simple_shell
Your Shell should:

Display a prompt and wait for the user to type a command. A command line always ends with a new line.
The prompt is displayed again each time a command has been executed.
The command lines are simple, no semicolons, no pipes, no redirections or any other advanced features.
The command lines are made only of one word. No arguments will be passed to programs.
If an executable cannot be found, print an error message and display the prompt again.
Handle errors.
You have to handle the “end of file” condition (Ctrl+D)
Handle command lines with arguments
Handle the PATH
fork must not be called if the command doesn’t exist
Implement the exit built-in, that exits the shell
Usage: exit
You don’t have to handle any argument to the built-in exit
Implement the env built-in, that prints the current environment
Write your own getline function
Use a buffer to read many chars at once and call the least possible the read system call
You will need to use static variables
You are not allowed to use getline
You are not allowed to use strtok
handle arguments for the built-in exit
Usage: exit status, where status is an integer used to exit the shell
Implement the setenv and unsetenv builtin commands

setenv
Initialize a new environment variable, or modify an existing one
Command syntax: setenv VARIABLE VALUE
Should print something on stderr on failure
unsetenv
Remove an environment variable
Command syntax: unsetenv VARIABLE
Should print something on stderr on failure
Implement the builtin command cd:

Changes the current directory of the process.
Command syntax: cd [DIRECTORY]
If no argument is given to cd the command must be interpreted like cd $HOME
You have to handle the command cd -
You have to update the environment variable PWD when you change directory
man chdir, man getcwd
Handle the commands separator ;
Handle the && and || shell logical operators
Implement the alias builtin command
Usage: alias [name[='value'] ...]
alias: Prints a list of all aliases, one per line, in the form name='value'
alias name [name2 ...]: Prints the aliases name, name2, etc 1 per line, in the form name='value'
alias name='value' [...]: Defines an alias for each name whose value is given. If name is already an alias, replaces its value with value
Handle variables replacement
Handle the $? variable
Handle the $$ variable
Handle comments (#)
Usage: simple_shell [filename]
Your shell can take a file as a command line argument
The file contains all the commands that your shell should run before exiting
The file should contain one command per line
In this mode, the shell should not print a prompt and should not read from stdin
