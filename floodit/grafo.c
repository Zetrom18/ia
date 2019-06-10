#include "grafo.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

//Cria uma lista vazia
lista* cria_lista(){
  lista *l = (lista *)malloc(sizeof(lista));
  assert(l);
  l->tam = 0;
  l->head = NULL;
  l->tail = NULL;
  return l;
}

//Adiciona item a lista
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

//Remove item da lista
void remove_item(lista *l, void *conteudo){
  item *aux;
  for(aux = l->head; aux!=NULL; aux = aux->prox){
    if(aux->conteudo==conteudo){
      aux->conteudo = NULL;
      if(aux == l->head){
        l->head = aux->prox;
        if(aux->prox != NULL){
          aux->prox->ante = NULL;
        }else{
          l->tail = NULL;
        }
      }else if(aux == l->tail){
        l->tail = aux->ante;
        if(aux->ante != NULL){
          aux->ante->prox = NULL;
        }else{
          l->head = NULL;
        }
      }else{
        aux->ante->prox = aux->prox;
        aux->prox->ante = aux->ante;
      }

      l->tam--;

      aux->conteudo = NULL;
      free(aux);
      return;
    }
  }
}

//Procura item em uma lista
item *acha_item(lista *l, void *conteudo){
  item *aux;
  for(aux = l->head; aux!=NULL; aux = aux->prox){
    if(aux->conteudo==conteudo){
      return aux;
    }
  }
  return NULL;
}

//Destroi e desaloca lista
void destroi_lista(lista *l){
  item *x;
  for(x = l->head; x!=NULL; x = l->head){
    remove_item(l, x->conteudo);
  }
  free(l);
}

//Cria um vértice
vertice *cria_vertice(int cor, int x, int y){
  vertice *v = (vertice *)malloc(sizeof(vertice));
  assert(v);
  v->cor = cor;
  v->tam = 1;
  v->coord_x = x+1;
  v->coord_y = y+1;
  v->vizinhos = cria_lista();
  return v;
}

//Destroi um vértice
void destroi_vertice(vertice *v){
  destroi_lista(v->vizinhos);
  free(v);
}

//Adiciona vizinhanca entre vértices
void adiciona_vizinho(vertice *a, vertice *b){
  if(a!=b){
    adiciona_item(a->vizinhos, b);
    adiciona_item(b->vizinhos, a);
  }
}

//Cria um grafo a partir de um map lido
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

//Destroi e desaloca um grafo
void destroi_grafo(grafo *g){
  item *x;
  vertice *v;
  for(x = g->vertices->head; g->vertices->head != NULL; x = g->vertices->head){
    v = (vertice *)x->conteudo;
    remove_item(g->vertices, (void *)v);
    destroi_vertice(v);
  }
  destroi_lista(g->vertices);
  free(g);
}

//Funde dois vértices de mesma cor
void fundir(grafo *g, vertice *a, vertice *b){
  while(b->vizinhos->tam != 0){
    if(acha_item(a->vizinhos, b->vizinhos->head->conteudo)==NULL){
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

//Aglutina todos os vértices próximos de mesma cor
void fundir_todos(grafo *g){
  item *aux, *vizinho, *vizinhoprox;
  for(aux = g->vertices->head; aux!=NULL; aux = aux->prox){
    vizinho = ((vertice *)aux->conteudo)->vizinhos->head;
    while(vizinho!=NULL){
      vizinhoprox = vizinho->prox;
      if(((vertice *)aux->conteudo)->cor == ((vertice *)vizinho->conteudo)->cor){
        fundir(g, (vertice *)aux->conteudo, (vertice *)vizinho->conteudo);
      }
      vizinho = vizinhoprox;
    }
  }
}

<<<<<<< HEAD:trabalho1/grafo.c
int checa_grafo(grafo *g){
  item *aux;
  int cont = 0;
  for(aux = g->vertices->head; aux!=NULL; aux = aux->prox){
    cont += ((vertice *)aux->conteudo)->tam;
  }
  return cont;
}

// no *cria_no(vertice *v){
//   no *n = (no *)malloc(sizeof(no));
//   assert(n);
//   n->conteudo = v;
//   n->filhos = cria_lista();
//   n->nivel = -1;
//   return n;
// }
//
// void destroi_no(no *n){
//   for(item *aux=n->filhos->head; aux!=NULL; aux=aux->prox){
//     destroi_no(aux->conteudo);
//   }
//   destroi_lista(n->filhos);
//   free(n);
// }
//
// arvore* cria_arvore(){
//   arvore *t = (arvore *)malloc(sizeof(arvore));
//   assert(t);
//   t->raiz = NULL;
//   t->altura = 0;
//   return t;
// }
//
// void destroi_arvore(arvore *t){
//   if(t!=NULL){
//     destroi_no(t->raiz);
//     free(t);
//   }
// }
//
// int adiciona_no(arvore *t, no *pai, no *filho){
//   if(pai == NULL){
//     t->raiz = filho;
//     filho->nivel = 0;
//   }else if(filho->nivel == -1 || pai->nivel < filho->nivel -1){
//     adiciona_item(pai->filhos, (void *)filho);
//     filho->nivel = pai->nivel + 1;
//   }else{
//     // adicao de um filho ja inserido na arvore nao sera implementada
//     puts("UNEXPECTED");
//   }
//   return filho->nivel;
// }
//
// no* acha_no(arvore *t, vertice *v){
//   lista *l = cria_lista();
//   adiciona_item(l, (void *)t->raiz);
//   for(item *aux = l->head; aux!=NULL; aux = l->head){
//     if(((vertice *)((no *)aux->conteudo)->conteudo) == v){
//       no *n = (no *)aux->conteudo;
//       destroi_lista(l);
//       return n;
//     }
//     for(item *x = ((no *)aux->conteudo)->filhos->head; x!=NULL; x = x->prox){
//       adiciona_item(l, (no *)x->conteudo);
//     }
//     remove_item(l, l->head->conteudo);
//   }
//   destroi_lista(l);
//   return NULL;
// }
//
// arvore* gera_arvore(vertice *v){
//   int altura;
//   item *aux, *vaux;
//   arvore *t = cria_arvore();
//   lista *l = cria_lista();
//   t->altura = 1 + adiciona_no(t, NULL, cria_no(v));
//   adiciona_item(l, (void *)v);
//   for(aux = l->head; aux!=NULL; aux = aux->prox){
//     for(vaux = ((vertice *)aux->conteudo)->vizinhos->head; vaux!=NULL; vaux = vaux->prox){
//       if(acha_item(l, (void *)vaux->conteudo)==NULL){
//         adiciona_item(l, (void *)vaux->conteudo);
//         no *a = acha_no(t, (vertice *)aux->conteudo);
//         no *b = cria_no((vertice *)vaux->conteudo);
//         altura = 1 + adiciona_no(t, a, b);
//         if(altura > t->altura){
//           t->altura = altura;
//         }
//       }
//     }
//   }
//   destroi_lista(l);
//   return t;
// }
//
// int checa_arvore(no *n){
//   int soma = n->conteudo->tam;
//   for(item *aux = n->filhos->head; aux != NULL; aux = aux->prox){
//     soma += checa_arvore((no *)aux->conteudo);
//   }
//   return soma;
// }
//
// arvore* encontra_melhor_arvore(grafo *g){
//   item *aux;
//   arvore *best=NULL, *taux;
//   int tam_best = MAX_INT;
//   for(aux = g->vertices->head; aux!=NULL; aux = aux->prox){
//     taux = gera_arvore((vertice *)aux->conteudo);
//     printf("%d\n", taux->altura); // DEBUG
//     if(taux->altura < tam_best){
//       destroi_arvore(best);
//       best = taux;
//       tam_best = best->altura;
//     }
//   }
//   return best;
// }
=======
>>>>>>> 38be630684e377cd66b68ce89fded02c5f1792df:floodit/grafo.c

//Encontra o melhor vértice para iniciar a pintura procurando o vértice de maior tamanho (mais células de mesma cor juntas)
vertice* encontra_melhor_vertice(grafo *g){
  int tamanho = 0;
  vertice *v = NULL;
  for(item *aux = g->vertices->head; aux!=NULL; aux = aux->prox){
    if(tamanho<((vertice *)aux->conteudo)->vizinhos->tam){
      tamanho = ((vertice *)aux->conteudo)->vizinhos->tam;
      v = (vertice *)aux->conteudo;
    }
  }
  return v;
}

//Aloca a estrutura do jogo para retorno
jogo* cria_jogo(vertice *v, int tam){
  jogo *j = (jogo *)malloc(sizeof(jogo));
  assert(j);
  j->x = v->coord_x;
  j->y = v->coord_y;
  j->n_jogadas = 0;
  j->jogadas = (int *)malloc(sizeof(int)*tam);
  assert(j->jogadas);
  return j;
}

<<<<<<< HEAD:trabalho1/grafo.c
int pega_altura(lista *l, vertice *v){
  int h;
  int altura = 1;
  for(item *aux=v->vizinhos->head; aux!=NULL; aux=aux->prox){
    if(acha_item(l, aux->conteudo)==NULL){
      adiciona_item(l, aux->conteudo);
      h = pega_altura(l, (vertice *)aux->conteudo) + 1;
      if(h > altura){
        altura = h;
      }
    }
  }
  return altura;
}

int busca_altura(vertice *v, int cor){
  puts("dale");
  int h;
  int altura=MAX_INT;
  lista *l = cria_lista();
  adiciona_item(l, (void *)v);
  for(item *aux=v->vizinhos->head; aux!=NULL; aux=aux->prox){
    adiciona_item(l, aux->conteudo);
  }
  for(item *aux=v->vizinhos->head; aux!=NULL; aux=aux->prox){
    if(((vertice *)aux->conteudo)->cor == cor){
      h = pega_altura(l, (vertice *)aux->conteudo);
    }else{
      h = pega_altura(l, (vertice *)aux->conteudo) + 1;
    }
    if(h < altura){
      altura = h;
    }
  }
  destroi_lista(l);
  return altura;
}

int encontra_melhor_cor(vertice *v){
  puts("oi?");
  int vizinhos[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  // for(item *aux = v->vizinhos->head; aux!=NULL; aux = aux->prox){
  //   vizinhos[((vertice *)aux->conteudo)->cor-1] += ((vertice *)aux->conteudo)->vizinhos->tam + 1;
  // }
  int melhor = 0;
  vizinhos[melhor] = busca_altura(v, 1);
  for(int i=1; i<10; i++){
    vizinhos[i] = busca_altura(v, i+1);
    if(vizinhos[melhor] < vizinhos[i]){
      melhor = i;
=======
//Encontra melhor cor para pintar o vértice raiz
//A heurística utilizada foi encontrar a cor com mais vizinhos de segundo grau (vizinho de vizinho) de mesma cor
int encontra_melhor_cor(vertice *v){
  int vizinhos[10][10] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
  };
  vertice *ver1, *ver2;
  item *aux, *bux;
  for(aux = v->vizinhos->head; aux!=NULL; aux = aux->prox){
    ver1 = ((vertice *)aux->conteudo);
    for (bux = ((vertice *)aux->conteudo)->vizinhos->head; bux!=NULL; bux = bux->prox) {
      ver2 = ((vertice *)bux->conteudo);
      vizinhos[ver1->cor - 1][ver2->cor - 1] += ver2->tam;
    }
  }
  int melhor1 = 0, melhor2 = 0;
  for(int i=0; i<10; i++){
    for(int j=0; j<10; j++) {
      if(vizinhos[melhor1][melhor2] < vizinhos[i][j]){
        melhor1 = i;
        melhor2 = j;
      }
>>>>>>> 38be630684e377cd66b68ce89fded02c5f1792df:floodit/grafo.c
    }
  }
  return melhor1+1;
}

//Pinta o vértice raíz
void pinta(grafo *g, vertice *v, int cor){
  vertice *b;
  item *aux = v->vizinhos->head;
  v->cor = cor;
  while(aux!=NULL){
    b = ((vertice *)aux->conteudo);
    aux = aux->prox;
    if (b->cor == cor) {
      while(b->vizinhos->tam != 0){
        if(acha_item(v->vizinhos, b->vizinhos->head->conteudo)==NULL){
          adiciona_vizinho(v, b->vizinhos->head->conteudo);
        }
        remove_item(((vertice *)b->vizinhos->head->conteudo)->vizinhos, b);
        remove_item(b->vizinhos, b->vizinhos->head->conteudo);
      }
      v->tam += b->tam;
      remove_item(g->vertices, b);
      free(b->vizinhos);
      free(b);
    }
  }
}

//Roda o algoritmo
jogo* floodit(vertice *v, grafo *g){
  int tam_max = 8, cor;
  jogo *j = cria_jogo(v, tam_max);
  while(v->vizinhos->tam > 0){
    puts("aqui?");
    cor = encontra_melhor_cor(v);
    pinta(g, v, cor);
    if(j->n_jogadas >= tam_max){
      tam_max = tam_max*2;
      j->jogadas = (int *)realloc(j->jogadas, sizeof(int)*tam_max);
      assert(j->jogadas);
    }
    j->jogadas[j->n_jogadas] = cor;
    j->n_jogadas++;
  }
  return j;
}

//Imprime a solução do algoritmo
void imprime_solucao(jogo *j){
  printf("%d %d %d\n", j->x, j->y, j->n_jogadas);
  for (int i = 0; i < j->n_jogadas; i++) {
    printf("%d ", j->jogadas[i]);
  }
  puts("");
}
