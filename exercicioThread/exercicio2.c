/* 
Considere o seguinte problema:
– Uma barbearia tem n barbeiros com suas respectivas cadeiras de barbeiro e m cadeiras para
clientes esperarem por sua vez na sala de espera.
– Quando não há clientes, o barbeiro se senta na cadeira e dorme.
– Quando chega um cliente:
• Se um barbeiro está disponível, ele precisa acordá-lo.
• Se todos os barbeiros estão ocupados, o cliente senta-se em uma das cadeiras na sala de espera e espera sua
vez.
• Se não há cadeiras disponíveis na sala de espera, o cliente vai embora.

• Implemente um programa usando threads, uma fila, uma única mutex e uma única variável
de condição (ou semáforo) para simular este problema.
– Argumentos:
• Quantidade de barbeiros, quantidade de cadeiras de espera, tempo de corte de cabelo (segundos), intervalo
entre chegada de clientes (segundos).
– Utilize uma thread para cada barbeiro.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>

#define MAX_CLIENTES 100

typedef struct {
    int id;
} Cliente;

int cadeirasEspera = 3;
int ocupadas = 0;
int tempoCorte = 2;
int numBarbeiros = 2;
int intervaloClientes = 1;
bool funcionando = true;

Cliente filaEspera[MAX_CLIENTES];

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condCliente = PTHREAD_COND_INITIALIZER;

void* barbeiro(void* arg) {
    int id = *(int*)arg;
    while (funcionando) {
        pthread_mutex_lock(&mutex);

        while (ocupadas == 0 && funcionando) {
            printf("Barbeiro %d dormindo.\n", id);
            pthread_cond_wait(&condCliente, &mutex);
        }

        if (!funcionando) {
            pthread_mutex_unlock(&mutex);
            break;
        }

        Cliente cliente = filaEspera[0];
        for (int i = 0; i < ocupadas - 1; ++i)
            filaEspera[i] = filaEspera[i + 1];
        ocupadas--;

        pthread_mutex_unlock(&mutex);

        printf("Barbeiro %d cortando o cabelo do cliente %d.\n", id, cliente.id);
        sleep(tempoCorte);
        printf("Barbeiro %d terminou de cortar o cabelo do cliente %d.\n", id, cliente.id);
    }
    printf("Barbeiro %d encerrando.\n", id);
    return NULL;
}

void* chegadaClientes(void* arg) {
    int intervalo = *(int*)arg;
    int clienteID = 1;

    while (funcionando) {
        sleep(intervalo);
        pthread_mutex_lock(&mutex);

        printf("Cliente %d chegou.\n", clienteID);

        if (ocupadas < cadeirasEspera) {
            filaEspera[ocupadas].id = clienteID;
            ocupadas++;
            pthread_cond_signal(&condCliente);
        } else {
            printf("Cliente %d foi embora sem cortar o cabelo. Sala de espera cheia.\n", clienteID);
        }

        clienteID++;
        pthread_mutex_unlock(&mutex);
    }
    printf("Encerrando chegada de clientes.\n");
    return NULL;
}

int main() {
    pthread_t threadsBarbeiros[numBarbeiros];
    int ids[numBarbeiros];

    for (int i = 0; i < numBarbeiros; ++i) {
        ids[i] = i + 1;
        pthread_create(&threadsBarbeiros[i], NULL, barbeiro, &ids[i]);
    }

    pthread_t threadClientes;
    pthread_create(&threadClientes, NULL, chegadaClientes, &intervaloClientes);

    sleep(10);

    pthread_mutex_lock(&mutex);
    funcionando = false;
    pthread_cond_broadcast(&condCliente); 
    pthread_mutex_unlock(&mutex);

    pthread_join(threadClientes, NULL);
    for (int i = 0; i < numBarbeiros; ++i)
        pthread_join(threadsBarbeiros[i], NULL);

    return 0;
}
