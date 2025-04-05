processo X:
  enquanto verdadeiro:
    item = produzir_item()
    P(vazio1)
    P(mutex1)
    insere(B1, item)
    V(mutex1)
    V(cheio1)

processo Y:
  enquanto verdadeiro:
    P(cheio1)
    P(mutex1)
    item = retira(B1)
    V(mutex1)
    V(vazio1)

    item_processado = processar_item(item)

    P(vazio2)
    P(mutex2)
    insere(B2, item_processado)
    V(mutex2)
    V(cheio2)

processo Z:
  enquanto verdadeiro:
    P(cheio2)
    P(mutex2)
    item = retira(B2)
    V(mutex2)
    V(vazio2)
    consumir_item(item)
