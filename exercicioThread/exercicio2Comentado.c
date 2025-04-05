#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>

// Definições de limites e configurações
#define MAX_CLIENTES 100 // Capacidade máxima da fila de espera

// Estrutura representando um cliente
typedef struct {
    int id; // Identificador único do cliente
} Cliente;

// Parâmetros de controle
int cadeirasEspera = 3;         // Número de cadeiras na sala de espera
int ocupadas = 0;               // Quantidade atual de cadeiras ocupadas
int tempoCorte = 2;             // Tempo (em segundos) que um barbeiro leva para cortar cabelo
int numBarbeiros = 2;           // Quantidade de barbeiros disponíveis
int intervaloClientes = 1;      // Intervalo (em segundos) entre a chegada de novos clientes
bool funcionando = true;        // Indica se a barbearia está em funcionamento

// Fila para armazenar os clientes esperando
Cliente filaEspera[MAX_CLIENTES];

// Sincronização entre threads
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;             // Mutex para controle de acesso à fila
pthread_cond_t condCliente = PTHREAD_COND_INITIALIZER;         // Variável de condição para notificar barbeiros

// Função da thread do barbeiro
void* barbeiro(void* arg) {
    int id = *(int*)arg; // ID do barbeiro

    while (funcionando) {
        pthread_mutex_lock(&mutex);

        // Se não há clientes na fila, barbeiro dorme
        while (ocupadas == 0 && funcionando) {
            printf("Barbeiro %d dormindo.\n", id);
            pthread_cond_wait(&condCliente, &mutex); // Aguarda sinal de que cliente chegou
        }

        // Caso o sistema esteja encerrando, sai do loop
        if (!funcionando) {
            pthread_mutex_unlock(&mutex);
            break;
        }

        // Pega o primeiro cliente da fila
        Cliente cliente = filaEspera[0];
        for (int i = 0; i < ocupadas - 1; ++i)
            filaEspera[i] = filaEspera[i + 1]; // Desloca os demais na fila
        ocupadas--; // Libera uma cadeira

        pthread_mutex_unlock(&mutex);

        // Realiza o corte de cabelo (simulado com sleep)
        printf("Barbeiro %d cortando o cabelo do cliente %d.\n", id, cliente.id);
        sleep(tempoCorte);
        printf("Barbeiro %d terminou de cortar o cabelo do cliente %d.\n", id, cliente.id);
    }

    printf("Barbeiro %d encerrando.\n", id);
    return NULL;
}

// Função da thread responsável pela chegada de clientes
void* chegadaClientes(void* arg) {
    int intervalo = *(int*)arg; // Intervalo entre chegadas
    int clienteID = 1;

    while (funcionando) {
        sleep(intervalo); // Espera pelo próximo cliente
        pthread_mutex_lock(&mutex);

        printf("Cliente %d chegou.\n", clienteID);

        if (ocupadas < cadeirasEspera) {
            // Se há cadeiras disponíveis, adiciona cliente à fila
            filaEspera[ocupadas].id = clienteID;
            ocupadas++;
            pthread_cond_signal(&condCliente); // Acorda um barbeiro, se necessário
        } else {
            // Caso contrário, cliente vai embora
            printf("Cliente %d foi embora sem cortar o cabelo. Sala de espera cheia.\n", clienteID);
        }

        clienteID++;
        pthread_mutex_unlock(&mutex);
    }

    printf("Encerrando chegada de clientes.\n");
    return NULL;
}

int main() {
    pthread_t threadsBarbeiros[numBarbeiros]; // Threads dos barbeiros
    int ids[numBarbeiros];                    // IDs para os barbeiros

    // Cria as threads dos barbeiros
    for (int i = 0; i < numBarbeiros; ++i) {
        ids[i] = i + 1;
        pthread_create(&threadsBarbeiros[i], NULL, barbeiro, &ids[i]);
    }

    // Cria a thread que gera a chegada de clientes
    pthread_t threadClientes;
    pthread_create(&threadClientes, NULL, chegadaClientes, &intervaloClientes);

    sleep(10); // Deixa a simulação rodar por 10 segundos

    // Finaliza a execução das threads
    pthread_mutex_lock(&mutex);
    funcionando = false;                      // Sinaliza fim do funcionamento
    pthread_cond_broadcast(&condCliente);     // Acorda todos os barbeiros para que terminem
    pthread_mutex_unlock(&mutex);

    // Aguarda término das threads
    pthread_join(threadClientes, NULL);
    for (int i = 0; i < numBarbeiros; ++i)
        pthread_join(threadsBarbeiros[i], NULL);

    return 0;
}
