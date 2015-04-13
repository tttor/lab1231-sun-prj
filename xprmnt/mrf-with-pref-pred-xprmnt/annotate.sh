#!/bin/bash
# if [ "$#" -ne 1 ]; then
#     echo "Illegal number of parameters"
#     exit 1
# fi

home=/home/tor
root_dir=$home/xprmnt/mrf-with-perf-pred-xprmnt-data
timestamp="$(date +'%Y%m%d.%H%M%S')"

img_id_list_filepath=$root_dir/meta/list01

dataset=voc2010
n_label=21
img_dir=$home/dataset/pascal/VOC2010/VOCdevkit/VOC2010/JPEGImages
img_extension=.jpg
unary_philipp_dir=$home/dataset/pascal/VOC2010/unary-philipp/voc2010_compressed

out_dir=$root_dir/annotation/mrf-with-perf-pred.$dataset.$timestamp
mkdir -p $out_dir
mkdir -p $out_dir/tmp # for tmp files for perf pred

superpixel_dir=$home/xprmnt/superpixel-generation/$dataset/dummy-segmentation
param_superpixel=-1dummyseg

knowledge_dir=$home/xprmnt/knowledge-construction
regression_dir=$home/xprmnt/knowledge-compatibility-benchmarker/regression-result
trained_estimator_filepath=$regression_dir/philippunarymrf-voc2010-NuSVR-20150324.104327/best_regressor_wrt_mse.pickle

# Annotate
exe=$home/lab1231-sun-prj/mrf-with-perf-pred/build/main

$exe $dataset $n_label \
	 $img_dir $img_extension $img_id_list_filepath \
	 $unary_philipp_dir $superpixel_dir $param_superpixel \
	 $knowledge_dir $trained_estimator_filepath \
	 $out_dir
	 