#ifndef MAX_INT
#define MAX_INT 32767
#endif

#ifndef _GRAFO_H
#define _GRAFO_H

#include "map.h"

//Estruturas de Dados Utilizadas:

//Item de uma Lista
typedef struct item {
  void *conteudo;
  struct item *ante;
  struct item *prox;
} item;

//tamanho_lista
typedef struct lista {
  item *head;
  item *tail;
  int tam;
} lista;

//Vértice de um grafo
typedef struct vertice {
  int cor;
  int tam;
  int coord_x;
  int coord_y;
  lista *vizinhos;
} vertice;

//Grafo
typedef struct grafo {
  lista *vertices;
} grafo;

//Resultado do algoritmo para retorno
typedef struct jogo {
  int x;
  int y;
  int n_jogadas;
  int *jogadas;
} jogo;

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
vertice* encontra_melhor_vertice(grafo *);
jogo* cria_jogo(vertice *, int);
int encontra_melhor_cor(vertice *);
void pinta(grafo *g, vertice *, int);
jogo *floodit(vertice *, grafo *);
void imprime_solucao(jogo *);

#endif
