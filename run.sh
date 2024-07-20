#!/bin/bash

case $1 in
    expHIN_origin0s)
        "./executable/${2:-hinscan}" ./data/expHIN_origin/ 0.68 3 0 "0 0 1 1 0" homoGraph.txt 0;;
    expHIN_origin1s)
        "./executable/${2:-hinscan}" ./data/expHIN_origin/ 0.68 3 1 "0 0 1 1 0" homoGraph.txt 0;;
    imdb0s)
        "./executable/${2:-hinscan}" ./data/imdb/ 0.82 4 0 "1 1 0 0 1" homoGraph0s.txt 0;;
    imdb1s)
        "./executable/${2:-hinscan}" ./data/imdb/ 0.82 4 1 "1 1 0 0 1" homoGraph1s.txt 0;;
    imdb0l)
        "./executable/${2:-hinscan}" ./data/imdb/ 0.84 15 0 "1 1 0 2 2 3 0 0 1" homoGraph0l.txt 2;;
    imdb1l)
        "./executable/${2:-hinscan}" ./data/imdb/ 0.84 15 1 "1 1 0 2 2 3 0 0 1" homoGraph1l.txt 2;;
    pubmed0s)
        "./executable/${2:-hinscan}" ./data/PubMed/ 0.51 2 0 "3 5 0 2 3" homoGraph0s.txt 1;;
    pubmed1s)
        "./executable/${2:-hinscan}" ./data/PubMed/ 0.51 2 1 "3 5 0 2 3" homoGraph1s.txt 1;;
    pubmed0l)
        "./executable/${2:-hinscan}" ./data/PubMed/ 0.96 200 0 "3 5 0 0 1 3 0 2 3" homoGraph0l.txt 2;;
    pubmed1l)
        "./executable/${2:-hinscan}" ./data/PubMed/ 0.96 200 1 "3 5 0 0 1 3 0 2 3" homoGraph1l.txt 2;;
    tmdb0s)
        "./executable/${2:-hinscan}" ./data/tmdb/ 0.51 2 0 "0 0 1 1 0" homoGraph0s.txt 1;;
    tmdb1s)
        "./executable/${2:-hinscan}" ./data/tmdb/ 0.51 2 1 "0 0 1 1 0" homoGraph1s.txt 1;;
    tmdb0l)
        "./executable/${2:-hinscan}" ./data/tmdb/ 0.96 400 0 "1 1 0 2 2 3 0 0 1" homoGraph0l.txt 2;;
    tmdb1l)
        "./executable/${2:-hinscan}" ./data/tmdb/ 0.64 3 1 "1 1 0 2 2 3 0 0 1" homoGraph1l.txt 2;;
    dblp0s)
        "./executable/${2:-hinscan}" ./data/dblp70w/ 0.72 4 0 "1 3 0 0 1" homoGraph0s.txt 1;;
    dblp1s)
        "./executable/${2:-hinscan}" ./data/dblp70w/ 0.72 4 1 "1 3 0 0 1" homoGraph1s.txt 1;;
    dblp0l)
        "./executable/${2:-hinscan}" ./data/dblp70w/ 0.98 500 0 "1 3 0 1 2 4 0 0 1" homoGraph0l.txt 2;;
    dblp1l)
        "./executable/${2:-hinscan}" ./data/dblp70w/ 0.64 3 1 "1 3 0 1 2 4 0 0 1" homoGraph1l.txt 2;;
    fortest0s)
        "./executable/${2:-hinscan}" ./data/ForTest/ 0.5 2 0 "0 0 1 3 0" homoGraph_short.txt 0;;
    fortest0l)
        "./executable/${2:-hinscan}" ./data/ForTest/ 0.5 2 0 "0 0 1 1 2 2 1 3 0" homoGraph_long.txt 0;;
esac
