#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

int readCount = 0;
int writeCount = 0;

sem_t mutexRead;    
sem_t mutexWrite;   
sem_t rw;         
sem_t tryRead;      

void* leitor(void* id) {
    int tid = *((int*)id);

    sem_wait(&tryRead);      
    sem_wait(&mutexRead);
    readCount++;
    if (readCount == 1)
        sem_wait(&rw);
    sem_post(&mutexRead);
    sem_post(&tryRead);

    printf("Leitor %d está lendo...\n", tid);
    sleep(1);
    printf("Leitor %d terminou de ler.\n", tid);

    sem_wait(&mutexRead);
    readCount--;
    if (readCount == 0)
        sem_post(&rw);
    sem_post(&mutexRead);

    pthread_exit(NULL);
}

void* escritor(void* id) {
    int tid = *((int*)id);

    sem_wait(&mutexWrite);
    writeCount++;
    if (writeCount == 1)
        sem_wait(&tryRead);
    sem_post(&mutexWrite);

    sem_wait(&rw);

    printf("Escritor %d está escrevendo...\n", tid);
    sleep(2);
    printf("Escritor %d terminou de escrever.\n", tid);

    sem_post(&rw);

    sem_wait(&mutexWrite);
    writeCount--;
    if (writeCount == 0)
        sem_post(&tryRead);
    sem_post(&mutexWrite);

    pthread_exit(NULL);
}

int main() {
    pthread_t leitores[5], escritores[3];
    int idsLeitores[5], idsEscritores[3];

    sem_init(&mutexRead, 0, 1);
    sem_init(&mutexWrite, 0, 1);
    sem_init(&rw, 0, 1);
    sem_init(&tryRead, 0, 1);

    for (int i = 0; i < 5; i++) {
        idsLeitores[i] = i + 1;
        pthread_create(&leitores[i], NULL, leitor, &idsLeitores[i]);
        sleep(1);

    for (int i = 0; i < 3; i++) {
        idsEscritores[i] = i + 1;
        pthread_create(&escritores[i], NULL, escritor, &idsEscritores[i]);
        sleep(2);
    }

    for (int i = 0; i < 5; i++)
        pthread_join(leitores[i], NULL);
    for (int i = 0; i < 3; i++)
        pthread_join(escritores[i], NULL);

    sem_destroy(&mutexRead);
    sem_destroy(&mutexWrite);
    sem_destroy(&rw);
    sem_destroy(&tryRead);

    return 0;
}
