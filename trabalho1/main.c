#include "grafo.h"
#include "map.h"
#include <stdio.h>

int main(int argc, char const *argv[]) {
  map_t *map = readMap();

  printMap(map);

  grafo *g = cria_grafo(map);

  fundir_todos(g);

  puts("Done");

  return 0;
}
