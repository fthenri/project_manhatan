#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define BUFFER_SIZE 9 // Tamanho do buffer compartilhado
#define NUM_CONSUMIDORES 3 // Número de consumidores
#define NUM_PRODUTORES 3 // Número de produtores

int BUFFER[BUFFER_SIZE]; // Buffer compartilhado
int in = 0, out = 0; // Índices para controle de entrada e saída do buffer
int count = 0; // Contador de itens no buffer

// Declaração de mutex e variáveis de condição
pthread_mutex_t mutex;
pthread_cond_t cond_produtor, cond_consumidor;

// Função do produtor
void *produtor(void *arg) {
    int id = *(int *)arg;
    for (int i = 0; i < 10; i++) { // Cada Produtor produzira 10 items e Cada Consumidor consumirá 10 items
        int item = rand() % 100; // Gera um item aleatório

        pthread_mutex_lock(&mutex);
        while (count == BUFFER_SIZE) { // Aguarda espaço disponível no buffer
            pthread_cond_wait(&cond_produtor, &mutex);
        }

        // Seção crítica (acesso ao buffer)
        BUFFER[in] = item;
        printf("Produtor %d produziu: %d (posição %d)\n", id, item, in);
        in = (in + 1) % BUFFER_SIZE;
        count++;

        pthread_cond_signal(&cond_consumidor); // Notifica consumidores
        pthread_mutex_unlock(&mutex);
        
        sleep(rand() % 2); // Simula o tempo de produção
    }
    return NULL;
}

// Função do consumidor
void *consumidor(void *arg) {
    int id = *(int *)arg;
    for (int i = 0; i < 10; i++) {
        pthread_mutex_lock(&mutex);
        while (count == 0) { // Aguarda um item disponível no buffer
            pthread_cond_wait(&cond_consumidor, &mutex);
        }

        // Seção crítica (acesso ao buffer)
        int item = BUFFER[out];
        printf("Consumidor %d consumiu: %d (posição %d)\n", id, item, out);
        out = (out + 1) % BUFFER_SIZE;
        count--;

        pthread_cond_signal(&cond_produtor); // Notifica produtores
        pthread_mutex_unlock(&mutex);
        
        sleep(rand() % 2); // Simula o tempo de consumo
    }
    return NULL;
}

int main() {
    pthread_t produtores[NUM_PRODUTORES], consumidores[NUM_CONSUMIDORES];
    int produtor_ids[NUM_PRODUTORES], consumidor_ids[NUM_CONSUMIDORES];

    // Inicialização do mutex e variáveis de condição
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond_produtor, NULL);
    pthread_cond_init(&cond_consumidor, NULL);

    // Criação das threads produtoras
    for (int i = 0; i < NUM_PRODUTORES; i++) {
        produtor_ids[i] = i + 1;
        pthread_create(&produtores[i], NULL, produtor, &produtor_ids[i]);
    }

    // Criação das threads consumidoras
    for (int i = 0; i < NUM_CONSUMIDORES; i++) {
        consumidor_ids[i] = i + 1;
        pthread_create(&consumidores[i], NULL, consumidor, &consumidor_ids[i]);
    }

    // Espera as threads produtoras finalizarem
    for (int i = 0; i < NUM_PRODUTORES; i++) {
        pthread_join(produtores[i], NULL);
    }

    // Espera as threads consumidoras finalizarem
    for (int i = 0; i < NUM_CONSUMIDORES; i++) {
        pthread_join(consumidores[i], NULL);
    }

    // Destruição do mutex e variáveis de condição
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_produtor);
    pthread_cond_destroy(&cond_consumidor);

    return 0;
}
