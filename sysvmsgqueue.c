#include <stdlib.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <pthread.h>
#include <stdio.h>

#define THREAD_COUNT 5

int id;

typedef struct buf {
	long mtype;
	char wtf;
} buf;

void *thread_task(void *in) {
	int i = *((int*) in);
	int a = i;
	struct buf data = {0};
	if (i == 0) {
		printf("dab");
		exit(-1);
	}
	printf("%d ts\n", i);
	if (msgrcv(id, &data, 0, i, 0) < 0) {
		perror("bad");
		exit(-1);
	}
	printf("%d rcvd\n", a);
	data.mtype = (i + 1) % (THREAD_COUNT + 1);
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
	data.mtype = 2;
	for(i = 0; i < THREAD_COUNT - 1; i++) {
		num[i] = i + 2;
		if (pthread_create(&threads[i], NULL, thread_task, &num[i]) != 0) {
			perror("bad");
			exit(-1);
		}
	}
	printf("Main thread starting dispatch %ld\n", data.mtype);
	if (msgsnd(id, &data, 0, 0) < 0) {
		perror("bad");
		exit(-1);
	}
	if (msgrcv(id, &data, 0, 1, 0) < 0) {
		perror("bad");
		exit(-1);
	}
	printf("Main thread received ok\n");
}
