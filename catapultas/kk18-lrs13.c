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

play *newJump(char player, char landing, int start_l, int start_c, int end_l, int end_c){
  play *aux = (play *)malloc(sizeof(play));
  aux->score = landing=='-' ? 0 : 1;
  aux->move[0] = player;
  aux->move[1] = ' ';
  aux->move[2] = 's';
  aux->move[3] = ' ';
  aux->move[4] = '2';
  aux->move[5] = ' ';
  aux->move[6] = '0'+start_l+1;
  aux->move[7] = ' ';
  aux->move[8] = '0'+start_c+1;
  aux->move[9] = ' ';
  aux->move[10] = '0'+end_l+1;
  aux->move[11] = ' ';
  aux->move[12] = '0'+end_c+1;
  aux->move[13] = '\0';
  return aux;
}

play **getAllPlays(info game, int start_l, int start_c){
  int playCount = 0;
  play **allPlays = (play **)calloc(MAXINT, sizeof(play *));

  // Check left
  if(start_c>0){
    if(game.table[start_l][start_c-1]=='-'){
      allPlays[playCount] = newMove(game.player, start_l, start_c, start_l, start_c-1);
      playCount++;
    }else if(game.table[start_l][start_c-1]==game.player &&
              start_c>1 &&
              game.table[start_l][start_c-2]!=game.player){
      allPlays[playCount] = newJump(game.player, game.table[start_l][start_c-2], start_l, start_c, start_l, start_c-2);
      playCount++;
    }
  }
  // Check right
  if(start_c<SIDSIZ-1){
    if(game.table[start_l][start_c+1]=='-'){
      allPlays[playCount] = newMove(game.player, start_l, start_c, start_l, start_c+1);
      playCount++;
    }else if(game.table[start_l][start_c+1]==game.player &&
              start_c<SIDSIZ-2 &&
              game.table[start_l][start_c+2]!=game.player){
      allPlays[playCount] = newJump(game.player, game.table[start_l][start_c+2], start_l, start_c, start_l, start_c+2);
      playCount++;
    }
  }
  // Check up
  if(start_l>0){
    if(game.table[start_l-1][start_c]=='-'){
      allPlays[playCount] = newMove(game.player, start_l, start_c, start_l-1, start_c);
      playCount++;
    }else if(game.table[start_l-1][start_c]==game.player &&
              start_l>1 &&
              game.table[start_l-2][start_c]!=game.player){
      allPlays[playCount] = newJump(game.player, game.table[start_l-2][start_c], start_l, start_c, start_l-2, start_c);
      playCount++;
    }
  }
  // Check down
  if(start_l<SIDSIZ-1){
    if(game.table[start_l+1][start_c]=='-'){
      allPlays[playCount] = newMove(game.player, start_l, start_c, start_l+1, start_c);
      playCount++;
    }else if(game.table[start_l+1][start_c]==game.player &&
              start_l<SIDSIZ-2 &&
              game.table[start_l+2][start_c]!=game.player){
      allPlays[playCount] = newJump(game.player, game.table[start_l+2][start_c], start_l, start_c, start_l+2, start_c);
      playCount++;
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
          if(best==NULL || playList[p]->score>best->score){
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
