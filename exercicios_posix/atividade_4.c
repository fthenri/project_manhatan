#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

pthread_rwlock_t rwlock;
pthread_mutex_t mutex;
int waiting_writers = 0;

void* leitor(void* arg) {
    int id = *((int*)arg);

    while (1) {
        pthread_mutex_lock(&mutex);
        while (waiting_writers > 0) {
            pthread_mutex_unlock(&mutex);
            usleep(100); 
            pthread_mutex_lock(&mutex);
        }
        pthread_mutex_unlock(&mutex);

        pthread_rwlock_rdlock(&rwlock);
        printf("Leitor %d está lendo...\n", id);
        sleep(1);
        printf("Leitor %d terminou de ler.\n", id);
        pthread_rwlock_unlock(&rwlock);

        sleep(rand() % 3); 
    }

    return NULL;
}

void* escritor(void* arg) {
    int id = *((int*)arg);

    while (1) {
        pthread_mutex_lock(&mutex);
        waiting_writers++;
        pthread_mutex_unlock(&mutex);

        pthread_rwlock_wrlock(&rwlock);
        printf("Escritor %d está escrevendo...\n", id);
        sleep(2);
        printf("Escritor %d terminou de escrever.\n", id);
        pthread_rwlock_unlock(&rwlock);

        pthread_mutex_lock(&mutex);
        waiting_writers--;
        pthread_mutex_unlock(&mutex);

        sleep(rand() % 4);
    }

    return NULL;
}

int main() {
    pthread_t leitores[5], escritores[2];
    int i, idsLeitores[5], idsEscritores[2];

    srand(time(NULL));
    pthread_rwlock_init(&rwlock, NULL);
    pthread_mutex_init(&mutex, NULL);

    for (i = 0; i < 5; i++) {
        idsLeitores[i] = i + 1;
        pthread_create(&leitores[i], NULL, leitor, &idsLeitores[i]);
    }

    for (i = 0; i < 2; i++) {
        idsEscritores[i] = i + 1;
        pthread_create(&escritores[i], NULL, escritor, &idsEscritores[i]);
    }

    for (i = 0; i < 5; i++) {
        pthread_join(leitores[i], NULL);
    }
    for (i = 0; i < 2; i++) {
        pthread_join(escritores[i], NULL);
    }

    pthread_rwlock_destroy(&rwlock);
    pthread_mutex_destroy(&mutex);

    return 0;
}
