#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <time.h>

#define CPU_CORES 4
#define ARR_SIZE 1000000000

typedef struct {
    int *arr;
    unsigned size;
} in_data;

typedef struct {
    long long sum;
} out_data;

typedef struct {
    in_data *in;
    out_data *out;
} allocated_memory;

void *thread_task(void *input) {
    in_data *cast_input = input;
    out_data *output = malloc(sizeof(out_data));
    unsigned long long i;
    output->sum = 0;
    for (i = 0; i < cast_input->size; i++) {
        output->sum += cast_input->arr[i];
    }
    free(input);
    return output;
}

int main() {
    unsigned long long i;
    int *arr = malloc(sizeof(int) * ARR_SIZE);
    in_data *input;
    out_data *result;
    clock_t time_before, time_after, time_after_2;
    long long sum = 0;
    pthread_t threads[CPU_CORES];
    for (i = 0; i < ARR_SIZE; i++) {
        arr[i] = i;
    }
    time_before = clock();
    for (i = 0; i < CPU_CORES; i++) {
        input = malloc(sizeof(in_data));
        input->size = ARR_SIZE / CPU_CORES;
        input->arr = arr + input->size * i;
        pthread_create(&threads[i], NULL, thread_task, input);
    }
    for (i = 0; i < CPU_CORES; i++) {
        pthread_join(threads[i], (void **)&result);
        sum += result -> sum;
        free(result);
    }
    time_after = clock();
    sum = 0;
    for (unsigned long long i = 0; i < ARR_SIZE; i++) {
        sum += arr[i];
    }
    time_after_2 = clock();
    free(arr);
    printf("%lu\n%lu\n", time_after - time_before, time_after_2 - time_after);
}
