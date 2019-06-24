CFLAGS = -g -Wall -Wno-unused-result -O3
LDLIBS = -l hiredis -l readline

all: controlador kk18-lrs13

controlador: controlador.c

kk18-lrs13: tabuleiro.h tabuleiro.c kk18-lrs13.c

clean:
	rm -f controlador kk18-lrs13
