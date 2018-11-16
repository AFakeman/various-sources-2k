#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>

#define TTY "/dev/ttys005"

pid_t other_pid;
FILE* tty;

int bit;

void parent_handler_usr1(int signal) {
    bit = 1;
}

void parent_handler_usr2(int signal) {
    bit = 0;
}

void empty_handler(int signal) {
    printf("empty handler\n");
}

void parent() {
    sigset_t mask;
    sigset_t oldmask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1 | SIGUSR2);
    sigprocmask(SIG_BLOCK, &mask, &oldmask);
    signal(SIGUSR1, parent_handler_usr1);
    signal(SIGUSR2, parent_handler_usr2);
    tty = fopen(TTY,  "w");
    while(1) {
        kill(other_pid, SIGUSR1);
        sigsuspend(&oldmask);
        fprintf(tty, "%d", bit);
    }
}

void child() {
    int num = 1488;
    sigset_t mask;
    sigset_t oldmask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1 | SIGUSR2);
    sigprocmask(SIG_BLOCK, &mask, &oldmask);
    signal(SIGUSR1, empty_handler);
    while(num > 0) {
        sigsuspend(&oldmask);
        bit = num % 2;
        num /= 2;
        if (bit) {
            kill(other_pid, SIGUSR1);
        } else {
            kill(other_pid, SIGUSR2);
        }
    }
}

int main() {
    pid_t parent_pid = getpid();
    other_pid = fork();
    if (other_pid < 0) {
        perror("fork");
        return -1;
    }
    if (!other_pid) {
        other_pid = parent_pid;
        parent();
    } else {
        child();
    }
    //printf("Not %d shutting down!\n", parent_pid);
}
