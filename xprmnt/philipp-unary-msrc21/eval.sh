#!/bin/sh
curr_dir="$(dirname "$0")"
. "$curr_dir/set_dataset_param.sh"

#
root_dir=/home/tor/sun4/xprmnt/philipp-unary-msrc21

pred_dir=$root_dir/annotation/test
target_listpath=$root_dir/meta/split-msrc-tor/Test_93.txt

#
exe=../../util/src-py/evaluator_tor.py 
python $exe $target_listpath $pred_dir $gt_csv_dir