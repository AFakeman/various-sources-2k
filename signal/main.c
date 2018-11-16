#include <signal.h>
#include <stdio.h>

void (*h) (int);

int counter = 5;

void handle(int arg) {
    if (!--counter) {
        signal(SIGINT, h);
    }
    printf("int\n");
}

int main() {
    h = signal(SIGINT, handle);
    while(1);
    return 0;
}
