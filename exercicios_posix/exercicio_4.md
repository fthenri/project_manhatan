typedef struct rv_t {
  semáforo sem_a = 0
  semáforo sem_b = 0
  inteiro count = 0
  mutex m
} rv_t

void rv_init(rv_t *rv) {
  inicializa(rv->sem_a, 0)
  inicializa(rv->sem_b, 0)
  inicializa(rv->m, 1)
  rv->count := 0
}

void rv_wait(rv_t *rv) {
  P(rv->m)
  rv->count := rv->count + 1
  se rv->count == 1 então
    V(rv->m)
    P(rv->sem_a)
    V(rv->sem_b)
  senão
    V(rv->sem_a)
    P(rv->sem_b)
    rv->count := 0
    V(rv->m)
}
