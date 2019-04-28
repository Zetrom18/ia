#!/bin/bash

gcc grafo.c map.c main.c -o teste -O3
./geramapa 100 100 10 > tmp.map
./teste < tmp.map > solucao.txt
./floodit_h2 < tmp.map > solucao_h2.txt
./floodit_h4 < tmp.map > solucao_h4.txt
./floodit_h6 < tmp.map > solucao_h6.txt
# cat tmp.map solucao.txt | ./anima

echo "nosso"
cat solucao.txt
echo "h2"
cat solucao_h2.txt
echo "h4"
cat solucao_h4.txt
echo "h6"
cat solucao_h6.txt
