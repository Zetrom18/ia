#ifndef _GRAFO_H
#define _GRAFO_H

#include "map.h"

typedef struct item {
  void *conteudo;
  struct item *ante;
  struct item *prox;
} item;

typedef struct lista {
  item *head;
  item *tail;
  int tam;
} lista;

typedef struct vertice {
  int cor;
  int tam;
  int coord_x;
  int coord_y;
  lista *vizinhos;
} vertice;

typedef struct grafo {
  lista *vertices;
} grafo;

typedef struct no {
  vertice *conteudo;
  lista *filhos;
} no;

typedef struct arvore {
  no *raiz;
  int altura;
} arvore;

lista* cria_lista();
void adiciona_item(lista *, void *);
void remove_item(lista *, void *);
item *acha_item(lista *, void *);
vertice *cria_vertice(int, int, int);
void adiciona_vizinho(vertice *, vertice *);
grafo* cria_grafo(map_t *);
void fundir_todos(grafo *);

#endif
