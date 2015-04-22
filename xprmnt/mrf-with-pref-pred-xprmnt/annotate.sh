#!/bin/bash
# if [ "$#" -ne 1 ]; then
#     echo "Illegal number of parameters"
#     exit 1
# fi
curr_dir="$(dirname "$0")"

home=/home/tor
root_dir=$home/xprmnt/mrf-with-perf-pred-xprmnt-data
timestamp="$(date +'%Y%m%d.%H%M%S')"

img_id_list_filepath=$root_dir/meta/list03
dataset=voc2010
n_label=21
. "$curr_dir/set_img_prop.sh"
. "$curr_dir/set_predictor_addr.sh"

unary_philipp_dir=$home/dataset/pascal/VOC2010/unary-philipp/voc2010_compressed
superpixel_dir=$home/xprmnt/superpixel-generation/$dataset/dummy-segmentation
param_superpixel=-1dummyseg

out_dir=$root_dir/annotation/mrf-with-perf-pred.$dataset.$timestamp
mkdir -p $out_dir
mkdir -p $out_dir/tmp # for tmp files for perf pred

# Annotate
echo "WARN: run the predictor server first!"
exe=$home/lab1231-sun-prj/mrf-with-perf-pred/build/main

$exe $dataset $n_label \
	 $img_dir $img_extension $img_id_list_filepath \
	 $unary_philipp_dir $superpixel_dir $param_superpixel \
	 $predictor_client_addr $out_dir
	 