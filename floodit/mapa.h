#ifndef __MAPA_H__
#define __MAPA_H__

typedef struct {
  int nlinhas;
  int ncolunas;
  int ncores;
  int lmax;
  int cmax;
  int lref;
  int cref;
  int **mapa;
} tmapa;

tmapa* aloca_mapa(tmapa*);
void libera_mapa(tmapa*);
void gera_mapa(tmapa*, int);
void carrega_mapa(tmapa*);
void mostra_mapa(tmapa*);
void mostra_mapa_cor(tmapa*);
void pinta_mapa(tmapa*, int);

#endif
