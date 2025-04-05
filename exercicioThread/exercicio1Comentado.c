#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <math.h>

// Definindo constantes
#define M_PI 3.14159265358979323846           // Valor de PI (caso não esteja definido)
#define TAMANHO_VETOR 10000000                // Tamanho do vetor a ser processado
#define NUM_THREADS 12                        // Número de threads a serem utilizadas

// Variáveis globais
double *vetor;                                // Vetor de valores aleatórios
double *resultados;                           // Vetor para armazenar resultados parciais de cada thread
double *tempos_thread;                        // Vetor para armazenar o tempo de execução de cada thread

// Função a ser aplicada em cada elemento do vetor
double funcao(double x) {
    return pow(2, -2 * pow((x - 0.1) / 0.9, 2)) * pow(sin(5 * M_PI * x), 6);
}

// Estrutura para passar argumentos para a thread
typedef struct {
    int id;                                   // ID da thread
    long inicio;                              // Índice inicial do vetor a ser processado
    long fim;                                 // Índice final do vetor a ser processado
} ThreadArgs;

// Função que será executada por cada thread
void* processar(void *arg) {
    ThreadArgs *args = (ThreadArgs*) arg;
    double soma_local = 0;

    // Marca o tempo de início da execução da thread
    struct timespec inicio, fim;
    clock_gettime(CLOCK_MONOTONIC, &inicio);

    // Processa parte do vetor, aplicando a função e acumulando os resultados
    for (long i = args->inicio; i < args->fim; i++) {
        soma_local += funcao(vetor[i]);
    }

    // Marca o tempo de fim da execução
    clock_gettime(CLOCK_MONOTONIC, &fim);
    double tempo_exec = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9;

    // Armazena os resultados da thread
    resultados[args->id] = soma_local;
    tempos_thread[args->id] = tempo_exec;

    pthread_exit(NULL); // Encerra a thread
}

int main() {
    // Aloca memória para os vetores
    vetor = malloc(sizeof(double) * TAMANHO_VETOR);
    resultados = malloc(sizeof(double) * NUM_THREADS);
    tempos_thread = malloc(sizeof(double) * NUM_THREADS);

    // Preenche o vetor com valores aleatórios entre 0 e 1
    for (long i = 0; i < TAMANHO_VETOR; i++) {
        vetor[i] = (double)rand() / RAND_MAX;
    }

    // Criação das threads e definição de seus argumentos
    pthread_t threads[NUM_THREADS];
    ThreadArgs args[NUM_THREADS];
    long intervalo = TAMANHO_VETOR / NUM_THREADS;

    struct timespec inicio_global, fim_global;
    clock_gettime(CLOCK_MONOTONIC, &inicio_global); // Tempo de início global

    // Cria as threads e define a faixa de dados que cada uma vai processar
    for (int i = 0; i < NUM_THREADS; i++) {
        args[i].id = i;
        args[i].inicio = i * intervalo;
        args[i].fim = (i == NUM_THREADS - 1) ? TAMANHO_VETOR : (i + 1) * intervalo;
        pthread_create(&threads[i], NULL, processar, &args[i]);
    }

    // Aguarda o término de todas as threads
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &fim_global); // Tempo de fim global

    // Soma todos os resultados parciais das threads
    double soma_total = 0;
    for (int i = 0; i < NUM_THREADS; i++) {
        soma_total += resultados[i];
    }

    // Calcula a média da função aplicada aos valores do vetor
    double media = soma_total / TAMANHO_VETOR;
    double tempo_total = (fim_global.tv_sec - inicio_global.tv_sec) + 
                         (fim_global.tv_nsec - inicio_global.tv_nsec) / 1e9;

    // Exibe o tempo de execução de cada thread
    for (int i = 0; i < NUM_THREADS; i++) {
        printf("Thread %d: Tempo = %.4f segundos\n", i, tempos_thread[i]);
    }

    printf("-------------------------------------\n");
    printf("Número de Threads: %d\n", NUM_THREADS);
    printf("Média global de f(x): %lf\n", media);
    printf("Tempo total de execução: %.3f segundos\n", tempo_total);

    // Libera a memória alocada
    free(vetor);
    free(resultados);
    free(tempos_thread);

    return 0;
}
