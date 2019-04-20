#ifndef _MAP_H
#define _MAP_H

typedef struct map_t{
  int lines;
  int columns;
  int colors;
  int **matrix;
  void ***pointers;
} map_t;

map_t* readMap(void);
void printMap(map_t*);
map_t* newMap(int, int, int);
void destroyMap(map_t *);

#endif
