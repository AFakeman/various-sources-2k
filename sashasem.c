#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdlib.h>
#include <stdio.h>

#define READERS 1000
#define START_SIZE 10

int main() {
    key_t mem_key, sem_key;
    int shmid, semid;
    struct sembuf ops[2];
    int i = 0;
    int *mem = 0;
    if ((mem_key = ftok("./header.h", 2)) < 0) {
        perror("bad mem ftok");
        return -1;
    }
    if ((sem_key = ftok("./header.h", 1)) < 0) {
        perror("bad sem ftok");
        return -1;
    }
    if ((shmid = shmget(mem_key, sizeof(int), IPC_CREAT | 0666)) < 0) {
        perror("bad shmget");
        return -1;
    }
    if ((mem = shmat(shmid, 0, 0)) == (int *) -1) {
        perror("bad shmat");
        shmctl(mem_key, IPC_RMID, NULL);
        return -1;
    }
    if ((semid = semget(sem_key, 2, IPC_CREAT | 0666)) < 0) {
        perror("bad semget");
        if (shmdt(mem) < 0) {
            perror("bad shmdt");
        }
        if (shmctl(shmid, IPC_RMID, NULL) < 0) {
            perror("bad shmctl");
        }
        return -1;
    }
    ops[0].sem_num = 0;
    ops[0].sem_op = READERS;
    ops[0].sem_flg = 0;
    ops[1].sem_num = 0;
    ops[1].sem_op = -READERS;
    ops[1].sem_flg = 0;
    *mem = START_SIZE;
    if (semop(semid, &ops[0], 1) < 0) {
        perror("bad semop 0");
        shmdt(mem);
        shmctl(shmid, IPC_RMID, NULL);
        semctl(semid, 2, IPC_RMID);
        return -1;
    }
    for(;;) {
        if (semop(semid, &ops[1], 1) < 0) {
            perror("bad semop 1");
            shmdt(mem);
            shmctl(shmid, IPC_RMID, NULL);
            semctl(semid, 2, IPC_RMID);
            return -1;
        }
        if (!--(*mem)) {
            break;
        }
        if (semop(semid, &ops[0], 1) < 0) {
            perror("bad semop 0");
            shmdt(mem);
            shmctl(shmid, IPC_RMID, NULL);
            semctl(semid, 2, IPC_RMID);
            return -1;
        }
    }
    shmdt(mem);
    shmctl(shmid, IPC_RMID, NULL);
    semctl(semid, 2, IPC_RMID);
}
