#!/bin/bash
# if [ "$#" -ne 1 ]; then
#     echo "Illegal number of parameters"
#     exit 1
# fi

home=/home/tor
root_dir=$home/xprmnt/ladicky-robustpn-voc
timestamp="$(date +'%Y%m%d.%H%M%S')"

img_list_filepath=$root_dir/meta/list00
ann_dir=$root_dir/ann/ladicky-robustpn-voc.$timestamp
mkdir -p $ann_dir

dataset=VOC2010
n_label=21
ori_img_dir=$home/dataset/pascal/$dataset/VOCdevkit/$dataset/JPEGImages
unary_philipp_dir=$home/dataset/pascal/$dataset/unary-philipp/voc2010_compressed

superpixel_dir=$home/xprmnt/superpixel-generation/one-superpixel
param_superpixel=onesuperpixel

# Annotate
exe=$home/lab1231-sun-prj/ladicky-2009/build/main 

$exe $dataset $n_label \
	 $ori_img_dir $img_list_filepath \
	 $ann_dir \
	 $unary_philipp_dir \
	 $superpixel_dir $param_superpixel
	 