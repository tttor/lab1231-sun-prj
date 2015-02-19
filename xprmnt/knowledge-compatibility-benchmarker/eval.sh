#!/bin/sh

curr_dir="$(dirname "$0")"
. "$curr_dir/set_dataset_param.sh"

root_dir=/home/tor/sun4/xprmnt/knowledge-compatibility-benchmarker-cvpr2015

pred_dir=$root_dir/annotation/annotation-random/ratio-0.0-1
target_listpath=$root_dir/meta/split_voc2010_tor/ann_img.list

exe=../../util/src-py/evaluator_tor.py 

python $exe $target_listpath $pred_dir $gt_csv_dir