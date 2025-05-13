#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/sem.h>
#include <sys/ipc.h>

#define NUM_PHILOSOPHERS 5

pthread_mutex_t forks[NUM_PHILOSOPHERS];
int sem_id;


void semaphore_wait() {
    struct sembuf op = {0, -1, 0};
    semop(sem_id, &op, 1);
}


void semaphore_signal() {
    struct sembuf op = {0, 1, 0};
    semop(sem_id, &op, 1);
}


void* philosopher(void* arg) {
    int id = *(int*)arg;
    int left = id;
    int right = (id + 1) % NUM_PHILOSOPHERS;

    while (1) {
        printf("Философ %d размышляет...\n", id);
        sleep(rand() % 3 + 1);

        semaphore_wait();

        pthread_mutex_lock(&forks[left]);
        pthread_mutex_lock(&forks[right]);

        printf("Философ %d ест (вилки %d и %d)\n", id, left, right);
        sleep(rand() % 2 + 1);

        pthread_mutex_unlock(&forks[right]);
        pthread_mutex_unlock(&forks[left]);

        semaphore_signal();
    }

    return NULL;
}


int main() {
    pthread_t philosophers[NUM_PHILOSOPHERS];
    int ids[NUM_PHILOSOPHERS];

    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_mutex_init(&forks[i], NULL);
    }

    sem_id = semget(IPC_PRIVATE, 1, IPC_CREAT | 0666);
    semctl(sem_id, 0, SETVAL, NUM_PHILOSOPHERS - 1)

    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        ids[i] = i;
        pthread_create(&philosophers[i], NULL, philosopher, &ids[i]);
    }

    pthread_join(philosophers[i], NULL);

    return 0;
}
