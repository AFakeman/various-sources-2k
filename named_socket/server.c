#include <sys/stat.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define BUF_SIZE 4096
#define PATH "./sock"
int main(int argc, char **argv) {
    int fifo;
    int pipe;
    int input;
    int rd, wr;
    char *buffer;
    umask(0);
    if (argc != 2) {
        fprintf(stderr, "Usage: program <file>");
        exit(1);
    }
    buffer = malloc(BUF_SIZE);
    if ((fifo = mkfifo(PATH, 0666)) < 0) {
        perror("Bad pipe open");
        free(buffer);
        exit(errno);
    }
    if ((pipe = open(PATH, O_WRONLY, 0666)) < 0) {
        perror("Bad pipe open");
        unlink(PATH);
        free(buffer);
        exit(errno);
    }
    if ((input = open(argv[1], O_RDONLY, 0666)) < 0) {
        perror("Bad input file open");
        close(pipe);
        unlink(PATH);
        free(buffer);
        exit(errno);
    }
    while (1) {
        if ((rd = read(input, buffer, BUF_SIZE)) < 0) {
            perror("Bad read");
            close(pipe);
            unlink(PATH);
            free(buffer);
            close(input);
            exit(errno);
        }
        if (rd == 0) {
            close(pipe);
            close(input);
            break;
        }
        if ((wr = write(pipe, buffer, rd)) < 0) {
            perror("Bad pipe write");
            close(pipe);
            unlink(PATH);
            free(buffer);
            close(input);
            exit(errno); 
        }
    }
   free(buffer);
}
