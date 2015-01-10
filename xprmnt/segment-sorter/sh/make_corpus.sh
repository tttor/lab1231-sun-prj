#!/bin/sh
timestamp="$(date +'%Y%m%d.%H%M%S')"
dataset=msrc21
region_desc=sift

#
root_dir=/home/jogie/sun4/exp/segment-sorter

list_filepath=$root_dir/meta/256.txt
segment_dir=$root_dir/superpixel/msrc/
clustered_region_dir=/home/jogie/sorter_exp/region-clustered/$dataset/$region_desc

corpus_dir=$root_dir/corpus-testing
corpus_filepath=$corpus_dir/corpus.testing.data
corpus_metafilepath=$corpus_dir/corpus.testing.meta

#
export PYTHONPATH=$PYTHONPATH:/home/jogie/git-jogie/lab1231-sun-prj/segment-sorter/common
exe=../../../segment-sorter/topic-discovery/make_corpus.py

python  $exe \
		$list_filepath $segment_dir $clustered_region_dir \
		$corpus_filepath $corpus_metafilepath
