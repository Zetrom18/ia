#!/bin/bash

gcc grafo.c map.c main.c -o teste -O3
./geramapa 10 10 3 > tmp.map
# time ./teste < tmp.map
time ./teste < tmp.map > solucao.txt
cat tmp.map solucao.txt | ./anima
