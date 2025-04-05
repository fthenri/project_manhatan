/* 
Faça um programa que:
– Receba como argumento a quantidade de threads
que devem ser criadas para a fase de
processamento.
– Crie um vetor x de 100.000.000 de elementos,
onde cada elemento é um número real com valor
aleatório entre 0 e 1.
– Para cada elemento xi do vetor, calcule f(xi) tal
que:
• f xi = 2
−2
xi−0,1
0,9
2
(sin 5πxi )
6

• Cada thread deve ficar responsável por uma faixa
determinada do vetor x.
– Divida igualmente o trabalho entre as threads
criadas.

– Ao final, o programa deve exibir:
• O valor médio de f xi para todos os xi ∈ x:
–
1
100.000.000
σi=1
100.000.000 f xi

• O tempo de execução em wall time (tempo do relógio).
– Não é necessário contar o tempo de execução da
geração do vetor x

• Compare o tempo de execução com diferentes
quantidades de threads:
– 1, 2, 4, 6, 8, 10, 12, 16, 24, 32, 64, 128, 256, ...
• Execute múltiplas vezes com cada quantidade para
obter uma média mais confiável.
• Responda as seguintes questões:
1. Qual o valor médio obtido para f xi ?
2. Qual foi o menor tempo de execução obtido?
3. Com quantas threads o tempo de execução foi o
menor?
4. Com quantas threads o tempo de execução
deixa de diminuir?
5. Quantos núcleos tem o processador da máquina
onde você executou os experimentos? Existe
alguma relação entre a quantidade de núcleos
do processador e as quantidades de threads
observadas nas questões anteriores?

*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <math.h>

#define M_PI 3.14159265358979323846
#define TAMANHO_VETOR 10000000
#define NUM_THREADS 12

double *vetor;
double *resultados;
double *tempos_thread;

double funcao(double x) {
    return pow(2, -2 * pow((x - 0.1) / 0.9, 2)) * pow(sin(5 * M_PI * x), 6);
}

typedef struct {
    int id;
    long inicio;
    long fim;
} ThreadArgs;

void* processar(void *arg) {
    ThreadArgs *args = (ThreadArgs*) arg;
    double soma_local = 0;

    struct timespec inicio, fim;
    clock_gettime(CLOCK_MONOTONIC, &inicio);

    for (long i = args->inicio; i < args->fim; i++) {
        soma_local += funcao(vetor[i]);
    }

    clock_gettime(CLOCK_MONOTONIC, &fim);
    double tempo_exec = (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9;

    resultados[args->id] = soma_local;
    tempos_thread[args->id] = tempo_exec;

    pthread_exit(NULL);
}

int main() {
    vetor = malloc(sizeof(double) * TAMANHO_VETOR);
    resultados = malloc(sizeof(double) * NUM_THREADS);
    tempos_thread = malloc(sizeof(double) * NUM_THREADS);

    // Gerando vetor aleatório
    for (long i = 0; i < TAMANHO_VETOR; i++) {
        vetor[i] = (double)rand() / RAND_MAX;
    }

    pthread_t threads[NUM_THREADS];
    ThreadArgs args[NUM_THREADS];
    long intervalo = TAMANHO_VETOR / NUM_THREADS;

    struct timespec inicio_global, fim_global;
    clock_gettime(CLOCK_MONOTONIC, &inicio_global);

    for (int i = 0; i < NUM_THREADS; i++) {
        args[i].id = i;
        args[i].inicio = i * intervalo;
        args[i].fim = (i == NUM_THREADS - 1) ? TAMANHO_VETOR : (i + 1) * intervalo;
        pthread_create(&threads[i], NULL, processar, &args[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &fim_global);

    double soma_total = 0;
    for (int i = 0; i < NUM_THREADS; i++) {
        soma_total += resultados[i];
    }

    double media = soma_total / TAMANHO_VETOR;
    double tempo_total = (fim_global.tv_sec - inicio_global.tv_sec) + 
                         (fim_global.tv_nsec - inicio_global.tv_nsec) / 1e9;

    // Impressão por thread
    for (int i = 0; i < NUM_THREADS; i++) {
        printf("Thread %d: Tempo = %.4f segundos\n", i, tempos_thread[i]);
    }

    printf("-------------------------------------\n");
    printf("Números de Threads: %d\n", NUM_THREADS);
    printf("Média global de f(x): %lf\n", media);
    printf("Tempo total de execução: %.3f segundos\n", tempo_total);

    free(vetor);
    free(resultados);
    free(tempos_thread);
    return 0;
}
