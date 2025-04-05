O impasse ocorre porque todos os filósofos podem pegar o garfo da esquerda ao mesmo tempo e ficarem esperando o da direita, que está ocupado pelo vizinho, formando um ciclo de espera sem fim.

void filosofo(int i) {
  while (1) {
    medita();
    if (i == N - 1) {
      sem_down(garfo[(i + 1) % N]);
      sem_down(garfo[i]);
    } else {
      sem_down(garfo[i]);
      sem_down(garfo[(i + 1) % N]);
    }
    come();
    sem_up(garfo[i]);
    sem_up(garfo[(i + 1) % N]);
  }
}
