#!/bin/bash

case $1 in
    expHIN_origin0s)
        ./hinscan ./data/expHIN_origin/ 0.68 3 0 "0 0 1 1 0" homograph.txt 0;;
    expHIN_origin1s)
        ./hinscan ./data/expHIN_origin/ 0.68 3 1 "0 0 1 1 0" homograph.txt 0;;
    imdb0s)
        ./hinscan ./data/imdb/ 0.64 3 0 "1 1 0 0 1" homograph.txt 0;;
    imdb1s)
        ./hinscan ./data/imdb/ 0.64 3 1 "1 1 0 0 1" homograph.txt 0;;
    imdb0l)
        ./hinscan ./data/imdb/ 0.64 3 0 "1 1 0 2 2 3 0 0 1" homograph.txt 0;;
    imdb1l)
        ./hinscan ./data/imdb/ 0.64 3 1 "1 1 0 2 2 3 0 0 1" homograph.txt 0;;
    pubmed0s)
        ./hinscan ./data/PubMed/ 0.64 3 0 "3 5 0 2 3" homograph.txt 0;;
    pubmed1s)
        ./hinscan ./data/PubMed/ 0.64 3 1 "3 5 0 2 3" homograph.txt 0;;
    pubmed0l)
        ./hinscan ./data/PubMed/ 0.64 3 0 "3 5 0 0 1 3 0 2 3" homograph.txt 0;;
    pubmed1l)
        ./hinscan ./data/PubMed/ 0.64 3 1 "3 5 0 0 1 3 0 2 3" homograph.txt 0;;
    tmdb0s)
        ./hinscan ./data/tmdb/ 0.64 3 0 "0 0 1 1 0" homograph.txt 0;;
    tmdb1s)
        ./hinscan ./data/tmdb/ 0.64 3 1 "0 0 1 1 0" homograph.txt 0;;
    tmdb0l)
        ./hinscan ./data/tmdb/ 0.64 3 0 "1 1 0 2 2 3 0 0 1" homograph.txt 0;;
    tmdb1l)
        ./hinscan ./data/tmdb/ 0.64 3 1 "1 1 0 2 2 3 0 0 1" homograph.txt 0;;
    dblp0s)
        ./hinscan ./data/dblp70w/ 0.64 3 0 "1 3 0 0 1" homograph.txt 0;;
    dblp1s)
        ./hinscan ./data/dblp70w/ 0.64 3 1 "1 3 0 0 1" homograph.txt 0;;
    dblp0l)
        ./hinscan ./data/dblp70w/ 0.64 3 0 "1 3 0 2 3 5 0 0 1" homograph.txt 0;;
    dblp1l)
        ./hinscan ./data/dblp70w/ 0.64 3 1 "1 3 0 2 3 5 0 0 1" homograph.txt 0;;

esac
