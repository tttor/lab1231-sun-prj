#!/bin/bash
# if [ "$#" -ne 1 ]; then
#     echo "Illegal number of parameters"
#     exit 1
# fi

home=/home/tor
root_dir=$home/xprmnt/ladicky-robustpn-voc
timestamp="$(date +'%Y%m%d.%H%M%S')"
with_perf_prediction=True

img_id_list_filepath=$root_dir/meta/list02
out_dir=$root_dir/annotation/ladicky-robustpn-voc.$timestamp
mkdir -p $out_dir

dataset=VOC2010
n_label=21
img_dir=$home/dataset/pascal/$dataset/VOCdevkit/$dataset/JPEGImages
img_extension=.jpg
unary_philipp_dir=$home/dataset/pascal/$dataset/unary-philipp/voc2010_compressed

superpixel_dir=$home/xprmnt/superpixel-generation/voc2010/dummy-segmentation
param_superpixel=-1dummyseg

# Annotate
exe=$home/lab1231-sun-prj/ladicky-2009/build/main 

$exe $dataset $n_label \
	 $img_dir $img_extension $img_id_list_filepath \
	 $unary_philipp_dir $superpixel_dir $param_superpixel \
	 $with_perf_prediction \
	 $out_dir
	 