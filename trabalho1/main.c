#include "grafo.h"
#include "map.h"
#include <stdio.h>

int main(int argc, char const *argv[]) {
  // puts("Start");
  puts("cade");
  map_t *map = readMap();

  // printMap(map);

  puts("cade");
  grafo *g = cria_grafo(map);

  // printf("Grafo gerado: %d\n", checa_grafo(g));

  puts("cade");
  fundir_todos(g);

  // printf("Grafo fundido: %d\n", checa_grafo(g));

  // arvore *t = encontra_melhor_arvore(g);
  // arvore *t = gera_arvore(encontra_melhor_vertice(g));
  puts("cade");
  vertice *v = encontra_melhor_vertice(g);

  // printf("Arvore gerada: %d\n", checa_arvore(t->raiz));

  puts("cade");
  imprime_solucao(floodit(v,g));
  // floodit(t);

  // printf("Arvore fundida: %d\n", checa_arvore(t->raiz));

  // destroi_arvore(t);

  destroi_grafo(g);

  // puts("Done");

  return 0;
}
