#include "grafo.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

lista* cria_lista(){
  lista *l = (lista *)malloc(sizeof(lista));
  assert(l);
  l->tam = 0;
  l->head = NULL;
  l->tail = NULL;
  return l;
}

void adiciona_item(lista *l, void *conteudo){
  item *x = (item *)malloc(sizeof(item));
  assert(x);
  x->conteudo = conteudo;
  x->ante = l->tail;
  x->prox = NULL;

  if(l->tam == 0){
    l->head = x;
    l->tail = x;
    l->tam = 1;
  }else{
    l->tail->prox = x;
    l->tail = x;
    l->tam++;
  }
}

void remove_item(lista *l, void *conteudo){
  item *aux;
  for(aux = l->head; aux!=NULL; aux = aux->prox){
    if(aux->conteudo==conteudo){
      if(aux == l->head){
        l->head = aux->prox;
        if(aux->prox != NULL){
          aux->prox->ante = NULL;
        }
      }else if(aux == l->tail){
        l->tail = aux->ante;
        if(aux->ante != NULL){
          aux->ante->prox = NULL;
        }
      }else{
        aux->ante->prox = aux->prox;
        aux->prox->ante = aux->ante;
      }

      l->tam--;

      free(aux);
      return;
    }
  }
}

item *acha_item(lista *l, void *conteudo){
  for(item *aux=l->head; aux!=NULL; aux=aux->prox){
    if(aux->conteudo==conteudo){
      return aux;
    }
  }
  return NULL;
}

vertice *cria_vertice(int cor, int x, int y){
  vertice *v = (vertice *)malloc(sizeof(vertice));
  assert(v);
  v->cor = cor;
  v->tam = 1;
  v->coord_x = x;
  v->coord_y = y;
  v->vizinhos = cria_lista();
  return v;
}

void adiciona_vizinho(vertice *a, vertice *b){
  if(a!=b){
    adiciona_item(a->vizinhos, b);
    adiciona_item(b->vizinhos, a);
  }
}

grafo* cria_grafo(map_t *map){
  vertice *v = NULL;
  grafo *g = (grafo *)malloc(sizeof(grafo));
  assert(g);
  g->vertices = cria_lista();

  for(int i=0; i<map->lines; i++){
    for(int j=0; j<map->columns; j++){
      v = (void*)cria_vertice(map->matrix[i][j], i, j);
      map->pointers[i][j] = (void *)v;
      adiciona_item(g->vertices, v);

      if(i>0){
        adiciona_vizinho(v, map->pointers[i-1][j]);
      }
      if(j>0){
        adiciona_vizinho(v, map->pointers[i][j-1]);
      }
      if(i == map->lines - 1){
        adiciona_vizinho(v, map->pointers[0][j]);
      }
    }
    adiciona_vizinho(v, map->pointers[i][0]);
  }

  destroyMap(map);

  return g;
}

void fundir(grafo *g, vertice *a, vertice *b){
  while(b->vizinhos->tam != 0){
    if(acha_item(a->vizinhos, b->vizinhos->head)==NULL){
      adiciona_vizinho(a, b->vizinhos->head->conteudo);
    }
    remove_item(((vertice *)b->vizinhos->head->conteudo)->vizinhos, b);
    remove_item(b->vizinhos, b->vizinhos->head->conteudo);
  }
  a->tam += b->tam;
  remove_item(g->vertices, b);
  free(b->vizinhos);
  free(b);
}

void fundir_todos(grafo *g){
  item *aux, *vizinho;
  for(aux = g->vertices->head; aux!=NULL; aux = aux->prox){
    for(vizinho = ((vertice *)aux->conteudo)->vizinhos->head; vizinho!=NULL; vizinho = vizinho->prox){
      if(((vertice *)aux->conteudo)->cor == ((vertice *)vizinho->conteudo)->cor){
        fundir(g, (vertice *)aux->conteudo, (vertice *)vizinho->conteudo);
      }
    }
  }
}
