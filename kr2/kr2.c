#include <sys/stat.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define PATH "./sock"

int main(int argc, char **argv) {
    int fifo;
    int pipe;
    size_t size;
    ssize_t rd, wr;
    char *buffer;
    umask(0);
    if (argc != 2) {
        printf("Usage: client <message>\n");
        exit(1);
    }
    size = strlen(argv[1]) + 1;
    if ((fifo = mkfifo(PATH, 0666)) < 0) {
        perror("Bad pipe open");
        exit(errno);
    }
    if ((pipe = open(PATH, O_RDWR, 0666)) < 0) {
        perror("Bad pipe open");
        unlink(PATH);
        exit(errno);
    }
    if ((wr = write(pipe, &size, sizeof(size))) < 0) {
        perror("Bad pipe size write");
        close(pipe);
        unlink(PATH);
        exit(errno);
    }
    if ((wr = write(pipe, argv[1], size)) < 0) {
        perror("Bad pipe msg write");
        close(pipe);
        unlink(PATH);
        exit(errno);
    }
    sleep(10);
    buffer = malloc(size);
    if (!buffer) {
        perror("Bad malloc");
        close(pipe);
        unlink(PATH);
        exit(errno);
    }
    if ((rd = read(pipe, buffer, size)) < 0) {
        perror("Bad read");
        close(pipe);
        unlink(PATH);
        free(buffer);
        exit(errno);
    }
    puts(buffer);
    free(buffer);
    close(pipe);
    unlink(PATH);
    return 0;
}
