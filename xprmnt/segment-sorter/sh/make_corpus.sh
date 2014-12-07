#!/bin/sh
timestamp="$(date +'%Y%m%d.%H%M%S')"

#
root_dir=/home/tor/xprmnt/segment-sorter

list_filepath=$root_dir/meta/msrc-21.sift.list
segment_dir=$root_dir/segment/msrc-21
clustered_region_dir=$root_dir/region-clustered/msrc-21

corpus_dir=$root_dir/corpus
corpus_filepath=$corpus_dir/corpus.$timestamp.data
corpus_metafilepath=$corpus_dir/corpus.$timestamp.meta

#
export PYTHONPATH=$PYTHONPATH:/home/tor/ws/lab1231-sun-prj/segment-sorter/common
exe=../../../segment-sorter/topic-discovery/make_corpus.py

python $exe $list_filepath $segment_dir $clustered_region_dir $corpus_filepath $corpus_metafilepath