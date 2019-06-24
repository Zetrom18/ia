CFLAGS = -g -Wall -Wno-unused-result -O3
LDLIBS = -l hiredis -l readline

all: controlador exemplo1 exemplo2 jogador kk18-lrs13

controlador: controlador.c

exemplo1: tabuleiro.h tabuleiro.c exemplo1.c

exemplo2: tabuleiro.h tabuleiro.c exemplo2.c

jogador: tabuleiro.h tabuleiro.c jogador.c

kk18-lrs13: tabuleiro.h tabuleiro.c kk18-lrs13.c

clean:
	rm -f controlador exemplo1 exemplo2 kk18-lrs13
