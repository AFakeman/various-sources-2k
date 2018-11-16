#include <sys/stat.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define PATH "./sock"
#define BUF_SIZE 4096

void str_reverse(char *str, size_t size) {
    size_t i;
    char temp;
    for(i = 0; i < size / 2; i++) {
        temp = str[i];
        str[i] = str[size - 1 - i];
        str[size - 1 - i] = temp;
    }
}

int main() {
    size_t size;
    int pipe;
    char *msg;
    while((pipe = open(PATH, O_RDONLY, 0666)) < 0) {
        if (errno != ENOENT) {
            perror("Could not open pipe");
            exit(1);
        }
    }
    if (read(pipe, &size, sizeof(size)) < 0) {
        perror("Bad size read");
        close(pipe);
        exit(errno);
    }
    printf("Allocating %ld...\n", size);
    msg = malloc(size);
    if (!msg) {
        perror("Bad malloc");
        close(pipe);
        exit(errno);
    }
    if (read(pipe, msg, size) < 0) {
        perror("Bad msg read");
        close(pipe);
        exit(errno);
    }
    close(pipe);
    str_reverse(msg, size - 1);
    if ((pipe = open(PATH, O_WRONLY, 0666)) < 0) {
        perror("Bad write open");
        free(msg);
    }
    if (write(pipe, msg, size) < 0) {
        perror("Bad msg write");
        close(pipe);
        free(msg);
        exit(errno);
    }
    close(pipe);
    free(msg);
}
