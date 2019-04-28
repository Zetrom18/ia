#!/bin/bash

gcc grafo.c map.c main.c -o teste
# ./geramapa 100 100 10 > tmp.map
# time ./teste < tmp.map
time ./teste < mapa_exemplo_100_100_10.txt
# cat tmp.map solucao.txt | ./anima
