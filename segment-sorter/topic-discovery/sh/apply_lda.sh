#!/bin/sh
#
lda=../../../external/GibbsLDA++-0.2/src/lda 
tr_data_dir=/home/tor/sun4/xprmnt/segment-sorter/lda-data/hot
tr_data_filename=msrc21.dat
tr_data_filepath=$tr_data_dir/$tr_data_filename

#
root_dir=/home/tor/sun4/xprmnt/segment-sorter/lda-model
run_id="$(date +'%Y%m%d.%H%M%S')"
target_dir=$root_dir/run.$run_id

rm -rf $target_dir
mkdir -p $target_dir
cp $tr_data_filepath $target_dir

#
dfile=$target_dir/$tr_data_filename
ntopics=21
twords=10

#$ lda -est [-alpha <double>] [-beta <double>] [-ntopics <int>] [-niters <int>] [-savestep <int>] [-twords <int>] -dfile <string>
$lda -est -dfile $dfile -ntopics $ntopics -twords $twords
