#ifndef MAX_INT
#define MAX_INT 32767
#endif

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
  int nivel;
} no;

typedef struct arvore {
  no *raiz;
  int altura;
} arvore;

lista* cria_lista();
void adiciona_item(lista *, void *);
void remove_item(lista *, void *);
item *acha_item(lista *, void *);
void destroi_lista(lista *);
vertice *cria_vertice(int, int, int);
void destroi_vertice(vertice *);
void adiciona_vizinho(vertice *, vertice *);
grafo* cria_grafo(map_t *);
void destroi_grafo(grafo *);
void fundir(grafo *, vertice *, vertice *);
void fundir_todos(grafo *);
int checa_grafo(grafo *);
no *cria_no(vertice *);
arvore* cria_arvore();
int adiciona_no(arvore *, no *, no *);
no* acha_no(arvore *, vertice *);
arvore* gera_arvore(vertice *);
arvore* econtra_melhor_arvore(grafo *);

#endif
