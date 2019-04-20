#!/bin/bash

gcc grafo.c map.c main.c -o teste -O3
./geramapa 100 100 3 > tmp.map
time ./teste < tmp.map

# gcc -lcgraph teste.c -o funcionaplz
# ./funcionaplz
