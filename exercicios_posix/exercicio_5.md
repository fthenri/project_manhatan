typedef struct barrier_t {
  inteiro total
  inteiro count
  semáforo sem = 0
  mutex m
} barrier_t

void barrier_init(barrier_t *barrier, inteiro N) {
  barrier->total := N
  barrier->count := 0
  inicializa(barrier->sem, 0)
  inicializa(barrier->m, 1)
}

void barrier_wait(barrier_t *barrier) {
  P(barrier->m)
  barrier->count := barrier->count + 1
  se barrier->count < barrier->total então
    V(barrier->m)
    P(barrier->sem)
  senão
    para i de 1 até barrier->total - 1 faça
      V(barrier->sem)
    barrier->count := 0
    V(barrier->m)
}
