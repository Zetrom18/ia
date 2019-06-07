#include "grafo.h"
#include "map.h"
#include <stdio.h>

int main(int argc, char const *argv[]) {
  map_t *map = readMap();

  grafo *g = cria_grafo(map);

  fundir_todos(g);

  vertice *v = encontra_melhor_vertice(g);

  imprime_solucao(floodit(v,g));

  destroi_grafo(g);

  return 0;
}
