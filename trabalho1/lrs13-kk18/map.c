#include "map.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

map_t* readMap(){
  int i,j;
  int n,m,c;
  assert(scanf("%d %d %d\n", &n, &m, &c) == 3);
  assert(n <= 100);
  assert(m <= 100);
  assert(c <= 10);
  map_t* map = newMap(n, m, c);

  for (i=0; i<n; i++)
    for (j=0; j<m; j++)
      assert(scanf("%d", &(map->matrix[i][j])) == 1);

  return map;
}

void printMap(map_t* m){
  int i, j;
  int size;
  if (m->colors < 10){
    size = 1;
  }else{
    size = 2;
  }

  for (i=0; i<m->lines; i++){
    for (j=0; j<m->columns; j++)
      printf("%*d ", size, m->matrix[i][j]);
    puts("");
  }
}

map_t* newMap(int n, int m, int c){
  int i;
  int** matrix = (int **)malloc(sizeof(int*)*n);
  assert(matrix);
  for (i=0; i<n; i++){
    matrix[i] = (int *)malloc(sizeof(int)*m);
    assert(matrix[i]);
  }

  void*** pointers = (void ***)malloc(sizeof(void **)*n);
  assert(pointers);
  for (i=0; i<n; i++){
    pointers[i] = (void **)malloc(sizeof(void *)*m);
    assert(pointers[i]);
  }

  map_t* map = (map_t*)malloc(sizeof(map_t));
  assert(map);
  map->lines = n;
  map->columns = m;
  map->colors = c;
  map->matrix = matrix;
  map->pointers = pointers;
  return map;
}

void destroyMap(map_t *m){
  int i,j;

  for(i=0; i<m->lines; i++){
    free(m->matrix[i]);
  }
  free(m->matrix);

  free(m);
}
