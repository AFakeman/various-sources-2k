#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#define BUF_SIZE 4096

int main(int argc, char **argv) {
    int pipe;
    int rd, wr;
    char *buffer;
    if (argc != 2) {
        fprintf(stderr, "Usage: program <pipe>");
        exit(1);
    }
    if ((pipe = open(argv[1], O_RDONLY, 0666)) < 0) {
        perror("Bad pipe open");
        exit(errno);
    }
    buffer = malloc(BUF_SIZE);
    while(1) {
        if ((rd = read(pipe, buffer, BUF_SIZE)) < 0) {
            perror("Bad pipe read");
            free(buffer);
            unlink(argv[1]);
            exit(errno);
        }
        if (rd == 0) {
            break;
        }
        if ((wr = write(1, buffer, rd)) < 0) {
            perror("Bad stdout write");
            free(buffer);
            unlink(argv[1]);
            exit(errno);
        }
    }
    free(buffer);
    close(pipe);
    unlink(argv[1]);
}
