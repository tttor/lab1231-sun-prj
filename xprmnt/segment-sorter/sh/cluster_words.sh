#!/bin/sh
#
root_dir=/home/tor/xprmnt/segment-sorter
timestamp="$(date +'%Y%m%d.%H%M%S')"

#
root_dir=/home/tor/xprmnt/segment-sorter

list_filepath=$root_dir/meta/msrc-21.sift.list
region_dir=$root_dir/region/msrc-21/sift

clustered_region_dir=$root_dir/region-clustered/msrc-21/sift
rm -rf $clustered_region_dir
mkdir -p $clustered_region_dir

kmeans_model_dir=$root_dir/kmeans-model
mkdir -p $kmeans_model_dir
kmeans_model_filepath=$kmeans_model_dir/kmeans.timestamp.model

#
n_word_cluster=2000
max_iter=300
n_init=10
init=k-means++

export PYTHONPATH=$PYTHONPATH:/home/tor/ws/lab1231-sun-prj/segment-sorter/common
exe=../../../segment-sorter/topic-discovery/cluster_words.py

python  $exe \
		$list_filepath $region_dir \
		$clustered_region_dir $kmeans_model_filepath \
		$n_word_cluster $max_iter $n_init $init