#!/bin/sh
#
root_dir=/home/tor/xprmnt/segment-sorter
run_id="$(date +'%Y%m%d.%H%M%S')"

#
corpus_name=corpus.20141207.085033
tr_data_filepath=$root_dir/corpus/$corpus_name.data
tr_metadata_filepath=$root_dir/corpus/$corpus_name.meta

model_dir=$root_dir/lda-model/training.$run_id
mkdir -p $model_dir
cp $tr_data_filepath $model_dir #as the LDAexe put the resulted model in the same dir as the inputted training_data
cp $tr_metadata_filepath $model_dir

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