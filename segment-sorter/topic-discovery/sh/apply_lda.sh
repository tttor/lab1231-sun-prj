#!/bin/sh
lda=../../../external/GibbsLDA++-0.2/src/lda 
dfile=/home/tor/sun4/xprmnt/segment-sorter/lda-data/msrc21.dat
ntopics=21
twords=10

#$ lda -est [-alpha <double>] [-beta <double>] [-ntopics <int>] [-niters <int>] [-savestep <int>] [-twords <int>] -dfile <string>
$lda -est -dfile $dfile -ntopics $ntopics -twords $twords
