#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define BUFFER_SIZE 9
#define NUM_CONSUMIDORES 3
#define NUM_PRODUTORES 3

int BUFFER[BUFFER_SIZE];
int in = 0, out = 0;
int count = 0;

pthread_mutex_t mutex;
pthread_cond_t cond_produtor, cond_consumidor;

void *produtor(void *arg) {
    int id = *(int *)arg;
    for (int i = 0; i < 10; i++) {
        int item = rand() % 100;

        pthread_mutex_lock(&mutex);
        while (count == BUFFER_SIZE) {
            pthread_cond_wait(&cond_produtor, &mutex);
        }

        BUFFER[in] = item;
        printf("Produtor %d produziu: %d (posição %d)\n", id, item, in);
        in = (in + 1) % BUFFER_SIZE;
        count++;

        pthread_cond_signal(&cond_consumidor);
        pthread_mutex_unlock(&mutex);
        
        sleep(rand() % 2);
    }
    return NULL;
}

void *consumidor(void *arg) {
    int id = *(int *)arg;
    for (int i = 0; i < 10; i++) {
        pthread_mutex_lock(&mutex);
        while (count == 0) {
            pthread_cond_wait(&cond_consumidor, &mutex);
        }

        int item = BUFFER[out];
        printf("Consumidor %d consumiu: %d (posição %d)\n", id, item, out);
        out = (out + 1) % BUFFER_SIZE;
        count--;

        pthread_cond_signal(&cond_produtor);
        pthread_mutex_unlock(&mutex);
        
        sleep(rand() % 2);
    }
    return NULL;
}

int main() {
    pthread_t produtores[NUM_PRODUTORES], consumidores[NUM_CONSUMIDORES];
    int produtor_ids[NUM_PRODUTORES], consumidor_ids[NUM_CONSUMIDORES];

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond_produtor, NULL);
    pthread_cond_init(&cond_consumidor, NULL);

    for (int i = 0; i < NUM_PRODUTORES; i++) {
        produtor_ids[i] = i + 1;
        pthread_create(&produtores[i], NULL, produtor, &produtor_ids[i]);
    }

    for (int i = 0; i < NUM_CONSUMIDORES; i++) {
        consumidor_ids[i] = i + 1;
        pthread_create(&consumidores[i], NULL, consumidor, &consumidor_ids[i]);
    }

    for (int i = 0; i < NUM_PRODUTORES; i++) {
        pthread_join(produtores[i], NULL);
    }

    for (int i = 0; i < NUM_CONSUMIDORES; i++) {
        pthread_join(consumidores[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_produtor);
    pthread_cond_destroy(&cond_consumidor);

    return 0;
}
