#!/bin/sh
#
root_dir=/home/tor/xprmnt/segment-sorter
run_id="$(date +'%Y%m%d.%H%M%S')"

#
tr_data_filename=corpus.20141207.085033.data
tr_data_filepath=$root_dir/corpus/$tr_data_filename

model_dir=$root_dir/lda-model/training.$run_id
mkdir -p $model_dir
cp $tr_data_filepath $model_dir #as the LDAexe put the resulted model in the same dir as the inputted training_data

#
dfile=$model_dir/$tr_data_filename
ntopics=21

alpha=0.5 #from [Russel, 2006]
beta=0.5 #from [Russel, 2006]

niters=100 #from [Russel, 2006]
savestep=20
twords=10

lda=../../../external/GibbsLDA++-0.2/src/lda 
$lda -est -dfile $dfile -ntopics $ntopics -alpha $alpha -beta $beta -niters $niters -savestep $savestep -twords $twords 