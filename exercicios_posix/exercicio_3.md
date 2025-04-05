semáforo sem_bart   = 1
semáforo sem_lisa   = 0
semáforo sem_maggie = 0

inteiro estado_lisa = 0

processo Bart:
  enquanto verdadeiro:
    P(sem_bart)
    move("Bart")
    V(sem_lisa)

processo Lisa:
  enquanto verdadeiro:
    P(sem_lisa)
    move("Lisa")
    se estado_lisa == 0 então
      estado_lisa := 1
      V(sem_maggie)
    senão
      estado_lisa := 0
      V(sem_bart)

processo Maggie:
  enquanto verdadeiro:
    P(sem_maggie)
    move("Maggie")
    V(sem_lisa)
