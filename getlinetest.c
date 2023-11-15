#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void custom_getline(char *buffer, size_t buffer_size) {
    ssize_t read_chars;
    size_t buffer_index = 0;
    size_t i;

    while (1) {
        if (buffer_index == 0) {
            read_chars = read(STDIN_FILENO, buffer, buffer_size);

            if (read_chars == 0) {
                break;
            } else if (read_chars == -1) {
                perror("read");
                exit(EXIT_FAILURE);
            }
       }

        for (i = buffer_index; i < buffer_size; ++i) {
            if (buffer[i] == '\n' || buffer[i] == '\0') {
                buffer[i] = '\0';
                buffer_index = i + 1;
                return;
            }
        }

        buffer_index = 0;
    }
}

