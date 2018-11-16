#include <pthread.h>
#include <stdio.h>

#define THREAD_COUNT 32

pthread_mutex_t mutex;

void *p_func(void *data) {
    int i = *(int *)data;
    pthread_mutex_unlock(&mutex);
    printf("hello from %lu\n", i);
    pthread_exit(NULL);
}

int main() {
    int i;
    pthread_t thid[THREAD_COUNT];
    pthread_mutex_init(&mutex, NULL);
    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_mutex_lock(&mutex);
        pthread_create(thid + i, NULL, p_func, &i);
    }
    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_join(thid[i], NULL);
    }
    pthread_mutex_destroy(&mutex);
    return 0;
}
