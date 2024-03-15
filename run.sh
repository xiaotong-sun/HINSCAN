#!/bin/bash

case $1 in
    expHIN_origin0s)
        ./hinscan ./data/expHIN_origin/ 0.68 3 0 "0 0 1 1 0" homograph.txt 0;;
    expHIN_origin1s)
        ./hinscan ./data/expHIN_origin/ 0.68 3 1 "0 0 1 1 0" homograph.txt 0;;
    imdb0s)
        ./hinscan ./data/imdb/ 0.64 6 0 "1 1 0 0 1" homograph.txt 0;;
    imdb1s)
        ./hinscan ./data/imdb/ 0.64 6 1 "1 1 0 0 1" homograph.txt 0;;
    imdb0l)
        ./hinscan ./data/imdb/ 0.64 6 0 "1 1 0 2 2 3 0 0 1" homograph.txt 0;;
    imdb1l)
        ./hinscan ./data/imdb/ 0.64 6 1 "1 1 0 2 2 3 0 0 1" homograph.txt 0;;

esac
