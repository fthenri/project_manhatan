#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

pthread_mutex_t mutex_area;
pthread_mutex_t mutex_contador_produtores;
pthread_mutex_t mutex_consumidor;
pthread_cond_t condicao_buffer;

int produtores_ativos = 0;
int sinal = 0;
int buffer[6] = {0, 0, 0, 0, 0, 0};
int indice_buffer = 0;

void* funcao_produtor(void* argumento)
{
    pthread_mutex_lock(&mutex_contador_produtores);
        produtores_ativos++;
        if (produtores_ativos == 1) pthread_mutex_lock(&mutex_area);
    pthread_mutex_unlock(&mutex_contador_produtores);

        int item = *((int*) argumento);
        printf("Produtor produzindo item: %d na posição %d\n", item, indice_buffer);
        buffer[indice_buffer] = item;

    pthread_mutex_lock(&mutex_contador_produtores);
        produtores_ativos--;
        if (produtores_ativos == 0) {
            pthread_mutex_unlock(&mutex_area);
            pthread_cond_broadcast(&condicao_buffer);
            sinal = 1;
        }
    pthread_mutex_unlock(&mutex_contador_produtores);

    return NULL;
}

void* funcao_consumidor(void* argumento)
{
    pthread_mutex_lock(&mutex_area);

    while (!sinal) {
        pthread_cond_wait(&condicao_buffer, &mutex_area);
    }

    pthread_mutex_lock(&mutex_consumidor);
        printf("Consumidor consumindo item: %d na posição %d\n", buffer[indice_buffer], indice_buffer);
        buffer[indice_buffer] = 0;
        indice_buffer++;
    pthread_mutex_unlock(&mutex_consumidor);

    pthread_mutex_unlock(&mutex_area);

    return NULL;
}

int main()
{
    pthread_t produtores[5];
    pthread_t consumidores[5];
    int itens[5] = {10, 20, 30, 40, 50};

    pthread_mutex_init(&mutex_area, NULL);
    pthread_mutex_init(&mutex_contador_produtores, NULL);
    pthread_mutex_init(&mutex_consumidor, NULL);
    pthread_cond_init(&condicao_buffer, NULL);

    for (int i = 0; i < 5; i++) {
        pthread_create(&produtores[i], NULL, funcao_produtor, &itens[i]);
    }

    for (int i = 0; i < 5; i++) {
        pthread_create(&consumidores[i], NULL, funcao_consumidor, NULL);
    }

    for (int i = 0; i < 5; i++) {
        pthread_join(produtores[i], NULL);
        pthread_join(consumidores[i], NULL);
    }

    printf("\nEstado final do buffer:\n");
    for (int i = 0; i < 6; i++) {
        printf("buffer[%d] = %d\n", i, buffer[i]);
    }

    return 0;
}
