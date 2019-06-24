#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "tabuleiro.h"

#define MAXSTR 512  // Maximum string size
#define MAXINT 1024 // Maximum int size
#define SIDSIZ 6    // Side size

struct info {
  char player ;
  int x ;
  int o ;
  char table[6][6] ;
};

struct play {
  int score ;
  char move[MAXSTR] ;
};

struct jump {
  int score ;
  int tCount ;
  int tuples[MAXINT][2] ;
};
//redefinindo a struct para encurtar o comando na declaração
typedef struct info info ;
typedef struct play play ;
typedef struct jump jump ;

info *newInfo(int argc, char **argv){
  if(argc<2){
    exit(1);
  }
  info *new = (info *)malloc(sizeof(info));
  new->player = argv[1][0];
  new->x = 18;
  new->o = 18;
  for(int l=0; l<SIDSIZ; l++){
    for(int c=0; c<SIDSIZ; c++){
      if(l<SIDSIZ/2){
        new->table[l][c] = 'x';
      }else{
        new->table[l][c] = 'o';
      }
    }
  }
  return new;
}

play *newMove(char player, int start_l, int start_c, int end_l, int end_c){
  play *aux = (play *)malloc(sizeof(play));
  aux->score = 0;
  aux->move[0] = player;
  aux->move[1] = ' ';
  aux->move[2] = 'm';
  aux->move[3] = ' ';
  aux->move[4] = '0'+start_l+1;
  aux->move[5] = ' ';
  aux->move[6] = '0'+start_c+1;
  aux->move[7] = ' ';
  aux->move[8] = '0'+end_l+1;
  aux->move[9] = ' ';
  aux->move[10] = '0'+end_c+1;
  aux->move[11] = '\0';
  return aux;
}

play *newJump(info game, jump *jp){
  int i;
  play *aux = (play *)malloc(sizeof(play));
  aux->score = jp->score;
  aux->move[0] = game.player;
  aux->move[1] = ' ';
  aux->move[2] = 's';
  aux->move[3] = ' ';
  aux->move[4] = '0'+jp->tCount;
  for(i=0; i<jp->tCount; i++){
    aux->move[5+(i*4)] = ' ';
    aux->move[6+(i*4)] = '0'+jp->tuples[i][0]+1;
    aux->move[7+(i*4)] = ' ';
    aux->move[8+(i*4)] = '0'+jp->tuples[i][1]+1;
  }
  aux->move[8+(i*4)]= '\0';
  return aux;
}

jump *copyJump(jump *src){
    jump *dst = (jump *)malloc(sizeof(jump));
    dst->score = src->score;
    dst->tCount = src->tCount;
    for(int i = 0; i<src->tCount; i++){
      dst->tuples[i][0] = src->tuples[i][0];
      dst->tuples[i][1] = src->tuples[i][1];
    }
    return dst;
}

bool isTuple(jump *baseJump, int a, int b){
  for(int i=0; i<baseJump->tCount; i++){
    if(baseJump->tuples[i][0]==a && baseJump->tuples[i][1]==b){
      return true;
    }
  }
  return false;
}

void goDeep(info game, jump **jl, int *jc, jump *bj, int l, int c){
  for(int i=0; i<*jc; i++){
    if(isTuple(jl[i], l, c)){
      return;
    }
  }
  jump *aux;
  // Check down
  if(l<SIDSIZ-2 && game.table[l+1][c]==game.player && game.table[l+2][c]!=game.player){
    aux = copyJump(bj);
    aux->score += game.table[l+2][c]=='-' ? 0 : 1;
    aux->tuples[aux->tCount][0] = l+2;
    aux->tuples[aux->tCount][1] = c;
    aux->tCount++;
    jl[(*jc)++] = aux;
    goDeep(game, jl, jc, aux, l+2, c);
  }
  // Check up
  if(l>1 && game.table[l-1][c]==game.player && game.table[l-2][c]!=game.player){
    aux = copyJump(bj);
    aux->score += game.table[l-2][c]=='-' ? 0 : 1;
    aux->tuples[aux->tCount][0] = l-2;
    aux->tuples[aux->tCount][1] = c;
    aux->tCount++;
    jl[(*jc)++] = aux;
    goDeep(game, jl, jc, aux, l-2, c);
  }
  // Check left
  if(c>1 && game.table[l][c-1]==game.player && game.table[l][c-2]!=game.player){
    aux = copyJump(bj);
    aux->score += game.table[l][c-2]=='-' ? 0 : 1;
    aux->tuples[aux->tCount][0] = l;
    aux->tuples[aux->tCount][1] = c-2;
    aux->tCount++;
    jl[(*jc)++] = aux;
    goDeep(game, jl, jc, aux, l, c-2);
  }
  // Check right
  if(c<SIDSIZ-2 && game.table[l][c+1]==game.player && game.table[l][c+2]!=game.player){
    aux = copyJump(bj);
    aux->score += game.table[l][c+2]=='-' ? 0 : 1;
    aux->tuples[aux->tCount][0] = l;
    aux->tuples[aux->tCount][1] = c+2;
    aux->tCount++;
    jl[(*jc)++] = aux;
    goDeep(game, jl, jc, aux, l, c+2);
  }
}

void addJumps(info game, play** playList, int *playCount, int start_l, int start_c){
  int jCount = 0;
  jump **jumpList = (jump **)calloc(MAXINT, sizeof(jump *));
  jump *baseJump = (jump *)malloc(sizeof(jump));
  baseJump->score = 0;
  baseJump->tCount = 1;
  baseJump->tuples[0][0] = start_l;
  baseJump->tuples[0][1] = start_c;

  goDeep(game, jumpList, &jCount, baseJump, start_l, start_c);

  for(int i=0; i<jCount; i++){
    playList[(*playCount)++] = newJump(game, jumpList[i]);
  }
}

play **getAllPlays(info game, int start_l, int start_c){
  int playCount = 0;
  play **allPlays = (play **)calloc(MAXINT, sizeof(play *));
  addJumps(game, allPlays, &playCount, start_l, start_c);

  // Check down
  if(start_l<SIDSIZ-1){
    if(game.table[start_l+1][start_c]=='-'){
      allPlays[playCount++] = newMove(game.player, start_l, start_c, start_l+1, start_c);
    }
  }
  // Check up
  if(start_l>0){
    if(game.table[start_l-1][start_c]=='-'){
      allPlays[playCount++] = newMove(game.player, start_l, start_c, start_l-1, start_c);
    }
  }
  // Check left
  if(start_c>0){
    if(game.table[start_l][start_c-1]=='-'){
      allPlays[playCount++] = newMove(game.player, start_l, start_c, start_l, start_c-1);
    }
  }
  // Check right
  if(start_c<SIDSIZ-1){
    if(game.table[start_l][start_c+1]=='-'){
      allPlays[playCount++] = newMove(game.player, start_l, start_c, start_l, start_c+1);
    }
  }
  allPlays[playCount] = NULL;
  return allPlays;
}

char *getBestPlay(info *game){
  play *best = NULL;
  play **playList;
  int count;

  if(game->player == 'x'){
    count = game->x;
  }else{
    count = game->o;
  }

  for(int i=0; i<SIDSIZ && count>0; i++){
    for(int j=0; j<SIDSIZ && count>0; j++){
      if(game->table[i][j]==game->player){
        playList = getAllPlays(*game, i, j);
        for(int p = 0; playList[p] != NULL; p++){
          if(best==NULL || playList[p]->score>best->score ||
         (playList[p]->move[2]=='s' && best->move[2]=='m') ||
         (playList[p]->move[2]=='s' && playList[p]->move[4]>best->move[4])){
            best = playList[p];
          }
        }
        count--;
      }
    }
  }

  puts(best->move);
  return best->move;
}

void updateGame(info *game, char *buf){
  int x = 0;
  int o = 0;
  char lado_meu;
  char lado_adv;
  char tipo_mov_adv;
  int l = -1;
  int c = 0;

  tabuleiro_recebe(buf);

  sscanf(strtok(buf, " \n"), "%c", &lado_meu);
  sscanf(strtok(NULL, " \n"), "%c", &lado_adv);
  sscanf(strtok(NULL, " \n"), "%c", &tipo_mov_adv);
  if(tipo_mov_adv != 'n') {
    strtok(NULL, "\n");
  }
  strncpy(buf, strtok(NULL, "."), MAXSTR);

  for(int i=0; buf[i]!='\0'; i++){
    if(buf[i]!='#'){
      if(buf[i]=='\n'){
        l++;
        c = 0;
      }else{
        game->table[l][c] = buf[i];
        if(buf[i]=='x'){
          x++;
        }
        if(buf[i]=='o'){
          o++;
        }
        c++;
      }
    }
  }

  game->x = x;
  game->o = o;
}

void printGame(info *game){
  printf("Peças Jogador x: %d\n", game->x);
  printf("Peças Jogador o: %d\n", game->o);
  puts("Tabuleiro:");
  for(int i=0; i<SIDSIZ; i++){
    for(int j=0; j<SIDSIZ; j++){
      printf("%c ", game->table[i][j]);
    }
    puts("");
  }
}

int main(int argc, char **argv) {
  char buf[MAXSTR];
  info *gameInfo = newInfo(argc, argv);

  tabuleiro_conecta(argc, argv);

  while(1) {
    updateGame(gameInfo, buf);
    // printGame(gameInfo);

    sprintf(buf, "%s\n", getBestPlay(gameInfo));

    tabuleiro_envia(buf);
  }
}
