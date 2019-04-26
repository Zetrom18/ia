#include "grafo.h"
#include "map.h"
#include <stdio.h>

int main(int argc, char const *argv[]) {
  map_t *map = readMap();

  // printMap(map);

  grafo *g = cria_grafo(map);

  printf("%d\n", checa_grafo(g));

  fundir_todos(g);

  printf("%d\n", checa_grafo(g));

  // arvore *t = encontra_melhor_arvore(g);
  arvore *t = gera_arvore(encontra_melhor_vertice(g));

  imprime_solucao(floodit(t));

  destroi_arvore(t);

  destroi_grafo(g);

  puts("Done");

  return 0;
}
