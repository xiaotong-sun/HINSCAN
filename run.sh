#!/bin/bash

case $1 in
    expHIN_origin0s)
        ./hinscan ./data/expHIN_origin/ 0.68 3 0 "0 0 1 1 0" homoGraph.txt 0;;
    expHIN_origin1s)
        ./hinscan ./data/expHIN_origin/ 0.68 3 1 "0 0 1 1 0" homoGraph.txt 0;;
    imdb0s)
        ./hinscan ./data/imdb/ 0.82 4 0 "1 1 0 0 1" homoGraph0s.txt 1;;
    imdb1s)
        ./hinscan ./data/imdb/ 0.82 4 1 "1 1 0 0 1" homoGraph1s.txt 1;;
    imdb0l)
        ./hinscan ./data/imdb/ 0.84 15 0 "1 1 0 2 2 3 0 0 1" homoGraph0l.txt 2;;
    imdb1l)
        ./hinscan ./data/imdb/ 0.84 15 1 "1 1 0 2 2 3 0 0 1" homoGraph1l.txt 2;;
    pubmed0s)
        ./hinscan ./data/PubMed/ 0.82 4 0 "3 5 0 2 3" homoGraph0s.txt 1;;
    pubmed1s)
        ./hinscan ./data/PubMed/ 0.82 4 1 "3 5 0 2 3" homoGraph1s.txt 1;;
    pubmed0l)
        ./hinscan ./data/PubMed/ 0.96 200 0 "3 5 0 0 1 3 0 2 3" homoGraph0l.txt 2;;
    pubmed1l)
        ./hinscan ./data/PubMed/ 0.96 200 1 "3 5 0 0 1 3 0 2 3" homoGraph1l.txt 2;;
    tmdb0s)
        ./hinscan ./data/tmdb/ 0.64 3 0 "0 0 1 1 0" homoGraph0s.txt 1;;
    tmdb1s)
        ./hinscan ./data/tmdb/ 0.64 3 1 "0 0 1 1 0" homoGraph1s.txt 1;;
    tmdb0l)
        ./hinscan ./data/tmdb/ 0.64 3 0 "1 1 0 2 2 3 0 0 1" homoGraph0l.txt 2;;
    tmdb1l)
        ./hinscan ./data/tmdb/ 0.64 3 1 "1 1 0 2 2 3 0 0 1" homoGraph1l.txt 2;;
    dblp0s)
        ./hinscan ./data/dblp70w/ 0.74 5 0 "1 3 0 0 1" homoGraph0s.txt 1;;
    dblp1s)
        ./hinscan ./data/dblp70w/ 0.74 5 1 "1 3 0 0 1" homoGraph1s.txt 1;;
    dblp0l)
        ./hinscan ./data/dblp70w/ 0.64 3 0 "1 3 0 2 3 5 0 0 1" homoGraph0l.txt 2;;
    dblp1l)
        ./hinscan ./data/dblp70w/ 0.64 3 1 "1 3 0 2 3 5 0 0 1" homoGraph1l.txt 2;;
    fortest0s)
        ./hinscan ./data/ForTest/ 0.5 2 0 "0 0 1 3 0" homoGraph_short.txt 0;;
    fortest0l)
        ./hinscan ./data/ForTest/ 0.5 2 0 "0 0 1 1 2 2 1 3 0" homoGraph_long.txt 0;;
esac
