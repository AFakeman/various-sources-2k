#include <stdlib.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <pthread.h>
#include <stdio.h>

#define THREAD_COUNT 1

int id;

typedef struct buf {
    long mtype;
    char wtf;
} buf;

void *thread_task(void *in) {
    int i = *((int*) in);
    int to;
    struct buf data = {0};
    if (msgrcv(id, &data, 0, i, 0) < 0) {
        perror("bad");
        exit(-1);
    }
    printf("%d rcvd\n", i);
    if (i % 2) {
        if (i == 1) {
            return NULL;
        } else {
            to = i - 2;
        }
    } else {
        if (i + 2 <= THREAD_COUNT) {
            to = i + 2;
        } else {
            if (i == THREAD_COUNT) {
                to = i - 1;
            } else {
                to = i + 1;
            }
        }
    }
    data.mtype = to;
    if (msgsnd(id, &data, 0, 0) < 0) {
        perror("bad");
        exit(-1);
    }
    printf("%ld snt\n", data.mtype);
    return NULL;
}

int main() {
    int i;
    int num[THREAD_COUNT];
    struct buf data;
    pthread_t threads[THREAD_COUNT];
    if ((id = msgget(IPC_PRIVATE, IPC_CREAT | 0666)) < 0) {
        perror("bad msgget");
    }
    for(i = 0; i < THREAD_COUNT; i++) {
        num[i] = i + 1;
        if (pthread_create(&threads[i], NULL, thread_task, &num[i]) != 0) {
            perror("bad");
            exit(-1);
        }
    }
    printf("Main thread starting dispatch\n");
    if (THREAD_COUNT == 1) {
        data.mtype = 1;
    } else {
        data.mtype = 2;
    }
    if (msgsnd(id, &data, 0, 0) < 0) {
        perror("bad");
        exit(-1);
    }
    for (i = 0; i < THREAD_COUNT; i++) {
        pthread_join(threads[i], NULL);
    }
    printf("Main thread outski\n");
}
