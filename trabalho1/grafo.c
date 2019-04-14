#include <graphviz/cgraph.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "grafo.h"

typedef struct no *no;
typedef struct lista *lista;
typedef struct vertice *vertice;
typedef struct aresta *aresta;

struct no {

  void *conteudo;
  no proximo;
};

struct lista {

  unsigned int tamanho;
  int padding; // só pra evitar warning
  no primeiro;
};

unsigned int tamanho_lista(lista l);
no primeiro_no(lista l);
no proximo_no(no n);
void *conteudo(no n);
no insere_lista(void *conteudo, lista l);
lista constroi_lista(void);
int destroi_lista(lista l, int destroi(void *));
int remove_no(struct lista *l, struct no *rno, int destroi(void *));
char *nome_grafo(grafo g);
unsigned int numero_vertices(grafo g);
unsigned int numero_arestas(grafo g);
char *nome_vertice(vertice v);
aresta busca_aresta(vertice v, vertice w, lista arestas);
lista vizinhanca(vertice v);
lista povoa_v_arestas(Agnode_t *v, Agraph_t *G);
lista povoa_vertices(Agraph_t *G);
lista povoa_arestas(Agraph_t *G);
int destroi_aresta(void *a);
int destroi_vertice(void *v);
void povoa_vizinhancas(grafo g, Agraph_t *G);
int busca_vizinhanca(lista vz, vertice v);
unsigned int grau(vertice v);
unsigned int get_color(unsigned int cor);
vertice maior_rotulo(lista V);

unsigned int tamanho_lista(lista l) { return l->tamanho; }

no primeiro_no(lista l) { return l->primeiro; }

no proximo_no(no n) { return n->proximo; }

void *conteudo(no n) { return n->conteudo; }

lista constroi_lista(void) {

  lista l = malloc(sizeof(struct lista));

  if ( ! l )
    return NULL;

  l->primeiro = NULL;
  l->tamanho = 0;

  return l;
}

int destroi_lista(lista l, int destroi(void *)) {

  no p;
  int ok=1;

  while ( (p = primeiro_no(l)) ) {

    l->primeiro = proximo_no(p);

    if ( destroi )
      ok &= destroi(conteudo(p));

    free(p);
  }

  free(l);

  return ok;
}

no insere_lista(void *conteudo, lista l) {

  no novo = malloc(sizeof(struct no));

  if ( ! novo )
    return NULL;

  novo->conteudo = conteudo;
  novo->proximo = primeiro_no(l);
  ++l->tamanho;

  return l->primeiro = novo;
}

int remove_no(struct lista *l, struct no *rno, int destroi(void *)) {
	int r = 1;
	if (l->primeiro == rno) {
		l->primeiro = rno->proximo;
		if (destroi != NULL) {
			r = destroi(conteudo(rno));
		}
		free(rno);
		l->tamanho--;
		return r;
	}
	for (no n = primeiro_no(l); n->proximo; n = proximo_no(n)) {
		if (n->proximo == rno) {
			n->proximo = rno->proximo;
			if (destroi != NULL) {
				r = destroi(conteudo(rno));
			}
			free(rno);
			l->tamanho--;
			return r;
		}
	}
	return 0;
}

//------------------------------------------------------------------------------
// (apontador para) estrutura de dados para representar um grafo
//
// o grafo pode ser direcionado ou não
//
// o grafo tem um nome, que é uma "string"

struct grafo {
  char *nome;

  lista vertices;
  lista arestas;
};

char *nome_grafo(grafo g){
  return g->nome;
}

unsigned int numero_vertices(grafo g){
  return tamanho_lista(g->vertices);
}

int n_vertices(grafo g){
  return (int )tamanho_lista(g->vertices);
}

unsigned int numero_arestas(grafo g){
  return tamanho_lista(g->arestas);
}

struct vertice {
  char *nome;
  unsigned int grau;
  unsigned int color;
  unsigned int rotulo;
  int padding;

  lista vizinhanca;
  lista arestas;
};

struct aresta {
  char *head;
  char *tail;

  unsigned int peso;
  int padding;
};

char *nome_vertice(vertice v){
  return v->nome;
}

aresta busca_aresta(vertice v, vertice w, lista arestas) {
  aresta a;
  for (no n = primeiro_no(arestas); n; n = proximo_no(n)) {
    a = (aresta)conteudo(n);
    if ((strcmp(nome_vertice(v),a->head) == 0 && strcmp(nome_vertice(w),a->tail) == 0) || (strcmp(nome_vertice(v),a->tail) == 0 && strcmp(nome_vertice(w),a->head) == 0)) {
      return a;
    }
  }
  return NULL;
}

vertice vertice_de_nome(char *s, grafo g) {
  lista vertices = g->vertices;
  for (no n = primeiro_no(vertices); n; n = proximo_no(n)){
    vertice v = conteudo(n);
    if (strcmp(v->nome,s) == 0)
      return v;
  }
  return NULL;
}

lista povoa_v_arestas(Agnode_t *v, Agraph_t *G){
  aresta a = NULL;
  lista arestas = constroi_lista();
  for (Agedge_t *e = agfstin(G,v); e; e = agnxtin(G,e)) {
    a = malloc(sizeof(struct aresta));
    a->head = malloc(sizeof(char) * 64);
    a->tail = malloc(sizeof(char) * 64);
    strcpy(a->head,agnameof(aghead(e)));
    strcpy(a->tail,agnameof(agtail(e)));
    if (insere_lista(a,arestas) == NULL){
      return NULL;
    }
  }
  return arestas;
}

lista povoa_vertices(Agraph_t *G){
  lista vertices = constroi_lista();
  vertice v;
  Agnode_t *n;
  for (n = agfstnode(G); n; n = agnxtnode(G,n)){
    v = malloc(sizeof(struct vertice));
    v->nome = malloc(sizeof(char)*64);
    strcpy(v->nome,agnameof(n));
    //grau e vizinhanca
    v->grau = (unsigned int)agdegree(G,n,TRUE,TRUE);
    v->arestas = povoa_v_arestas(n,G);
    v->color = 0;
    insere_lista(v,vertices);
  }

  return vertices;
}

int busca_vizinhanca(lista vz, vertice v){
  for (no n = primeiro_no(vz); n; n = proximo_no(n)){
    vertice w = conteudo(n);
    if (v == w)
      return 1;
  }
  return 0;
}

void povoa_vizinhancas(grafo g, Agraph_t *G){
  vertice w, v;
  Agnode_t *n;
  Agedge_t *e;
  for (n = agfstnode(G); n; n = agnxtnode(G,n)){
    v = vertice_de_nome(agnameof(n), g);

    v->vizinhanca = constroi_lista();
    for (e = agfstedge(G,n); e; e = agnxtedge(G,e,n)){
      w = vertice_de_nome(agnameof(e->node), g);
      insere_lista(w,v->vizinhanca);
    }
  }
  return;
}

lista povoa_arestas(Agraph_t *G){
  aresta a = NULL;
  lista arestas = constroi_lista();
  for (Agnode_t *n = agfstnode(G); n; n = agnxtnode(G,n)) {
    for (Agedge_t *e = agfstout(G,n); e; e = agnxtout(G,e)) {
      a = malloc(sizeof(struct aresta));
      a->head = malloc(sizeof(char)*64);
      a->tail = malloc(sizeof(char)*64);
      strcpy(a->head,agnameof(aghead(e)));
      strcpy(a->tail,agnameof(agtail(e)));
      if (insere_lista(a,arestas) == NULL){
        return NULL;
      }
    }
  }
  return arestas;
}

int destroi_aresta(void *a){
  free(a);
  return 1;
}

int destroi_vertice(void *v){
  int ok = 1;
  vertice w = (vertice)v;
  if (w->vizinhanca != NULL){
    ok &= destroi_lista(w->vizinhanca, NULL);
  }
  ok &= destroi_lista(w->arestas, destroi_aresta);
  free(v);
  return ok;
}

//------------------------------------------------------------------------------
// desaloca toda a memória usada em *g
//
// devolve 1 em caso de sucesso,
//         ou
//         0, caso contrário

int destroi_grafo(grafo g) {
  int ok = 1;
  ok &= destroi_lista(g->vertices, destroi_vertice);
  ok &= destroi_lista(g->arestas, destroi_aresta);
  free(g);
  return ok;
}
//------------------------------------------------------------------------------
// lê um grafo no formato dot de input
//
// devolve o grafo lido,
//         ou
//         NULL, em caso de erro

grafo le_grafo(FILE *input){
	grafo g = malloc(sizeof(struct grafo));
	Agraph_t *G = agread(input, NULL);

  g->vertices = povoa_vertices(G);
  g->arestas = povoa_arestas(G);
  povoa_vizinhancas(g,G);

  //nome
  g->nome = malloc(sizeof(char)*64);
  strcpy(g->nome,agnameof(G));
  agclose(G);
	return g;
}
//------------------------------------------------------------------------------
// escreve o grafo g em output usando o formato dot.
//
// devolve o grafo escrito,
//         ou
//         NULL, em caso de erro

grafo escreve_grafo(FILE *output, grafo g){
  unsigned int i;
  int ok = 1;
  no n;
  ok &= fprintf(output, "graph %s {\n\n",
         g->nome
       );
  if (numero_vertices(g)) {
    n = primeiro_no(g->vertices);
    vertice v = (vertice)conteudo(n);
    ok &= fprintf(output, "    %s", v->nome);
    if (v->color){
      ok &= fprintf(output, " [color=#%X]", get_color(v->color));
    }

    ok &= fprintf(output, "\n");
    for (i = 0; i < (numero_vertices(g) - 1); ++i){
      n = proximo_no(n);
      v = (vertice)conteudo(n);
      ok &= fprintf(output, "    %s", v->nome);
      if (v->color){
        ok &= fprintf(output, " [color=#%X]", get_color(v->color));
      }

      ok &= fprintf(output, "\n");
    }
  }

  ok &= fprintf(output, "\n");

  if (numero_arestas(g)){
    n = primeiro_no(g->arestas);
    aresta a = (aresta)conteudo(n);
    ok &= fprintf(output, "    %s -- %s", a->tail, a->head);

    ok &= fprintf(output, "\n");

    for (i = 0; i < (numero_arestas(g) - 1); ++i){
      n = proximo_no(n);
      a = (aresta)conteudo(n);
      ok &= fprintf(output, "    %s -- %s", a->tail, a->head);

      ok &= fprintf(output, "\n");

    }
  }
  ok &= fprintf(output, "}\n");

  if (ok){
    return g;
  }else{
    return NULL;
  }
}

lista vizinhanca(vertice v){
  return v->vizinhanca;
}

unsigned int grau(vertice v){
  return v->grau;
}

//a partir daqui é só a parte específica do trabalho de grafos

// unsigned int get_color(unsigned int cor) {
//   //lista estática de cores
//   unsigned int lista_de_cores[32] = {
//     0, 16776960, 1894143, 16725247, 16730694, 35137, 28582, 10682457,
//     16767973, 8014080, 166, 6553516, 12031842, 19779, 9416959, 10059143,
//     5898247, 8427155, 16711654, 1786880, 5228033, 3890687, 4864851, 16723840,
//     6381914, 12192000, 7043328, 49824, 16755346, 16748745, 12125098, 13721856
//   };
//   return lista_de_cores[cor];
// }
//
// //------------------------------------------------------------------------------
// // devolve um número entre 0 e o número de vertices de g
//
// unsigned int cor(vertice v, grafo g) {
//   if (v->color <= tamanho_lista(g->vertices)) {
//     return v->color;
//   }
//   return 0;
// }
//
// //------------------------------------------------------------------------------
// // preenche o vetor v (presumidamente um vetor com n_vertices(g)
// // posições) com os vértices de g ordenados de acordo com uma busca em
// // largura lexicográfica sobre g a partir de r e devolve v
//
// vertice *busca_lexicografica(vertice r, grafo g, vertice *v) {
//   lista S, V = constroi_lista();
//   vertice z, w, x, *res;
//   no m, n = primeiro_no(g->vertices);
//   while (n != NULL) {
//     z = conteudo(n);
//     z->rotulo = 0;
//     insere_lista(z,V);
//     n = proximo_no(n);
//   }
//   S = constroi_lista();
//   insere_lista(r, S);
//   n = primeiro_no(V);
//   z = conteudo(n);
//   for (n = primeiro_no(V); n; n = proximo_no(n)) {
//     z = conteudo(n);
//     if (r == z) {
//       remove_no(V,n,NULL);
//     }
//   }
//
//   r->rotulo = tamanho_lista(g->vertices);
//   for (n = primeiro_no(r->vizinhanca); n; n = proximo_no(n)) {
//     z = conteudo(n);
//     z->rotulo = r->rotulo - 1;
//   }
//   while(tamanho_lista(V) > 0) {
//     z = maior_rotulo(V);
//     insere_lista(z,S);
//     for (n = primeiro_no(V); n; n = proximo_no(n)) {
//       w = conteudo(n);
//       if (z == w) {
//         remove_no(V,n,NULL);
//         break;
//       }
//     }
//     for (n = primeiro_no(z->vizinhanca); n; n = proximo_no(n)) {
//       w = conteudo(n);
//       for (m = primeiro_no(V); m; m = proximo_no(m)) {
//         x = conteudo(m);
//         if (x == w && w->rotulo == 0) {
//           w->rotulo = z->rotulo - 1;
//         }
//       }
//     }
//   }
//   res = malloc(sizeof(struct vertice) * tamanho_lista(S));
//   int i = 0;
//   for (n = primeiro_no(S); n; n = proximo_no(n), i++) {
//     res[i] = conteudo(n);
//   }
//   return res;
// }
//
// vertice maior_rotulo(lista V) {
//   vertice v, w;
//   for (no n = primeiro_no(V); n; n = proximo_no(n)) {
//     w = conteudo(n);
//     if (v == NULL) {
//       v = w;
//     } else {
//       if (w->rotulo > v->rotulo) {
//         v = w;
//       }
//     }
//   }
//   if (v == NULL) {
//     return w;
//   }
//   return v;
// }
//
// //------------------------------------------------------------------------------
// // colore os vértices de g de maneira "gulosa" segundo a ordem dos
// // vértices em v e devolve o número de cores utilizado
// //
// // ao final da execução,
// //     1. cor(v,g) > 0 para todo vértice de g
// //     2. cor(u,g) != cor(v,g), para toda aresta {u,v} de g
//
// unsigned int colore(grafo g, vertice *v) {
//   vertice w, z;
//   no n;
//   unsigned int hasColor, maxColor = 0;
//   for (unsigned int i = 0; i < tamanho_lista(g->vertices); i++) {
//     w = v[i];
//     for (unsigned int j = 1; j <= tamanho_lista(g->vertices); j++) {
//       if (j > maxColor) {
//         maxColor = j;
//       }
//       hasColor = 0;
//       for (n = primeiro_no(vizinhanca(w)); n; n = proximo_no(n)) {
//         z = conteudo(n);
//         if (z->color == j) {
//           hasColor = 1;
//           break;
//         }
//       }
//       if (hasColor == 0) {
//         w->color = j;
//         break;
//       }
//     }
//   }
//   return maxColor;
// }

//------------------------------------------------------------------------------
