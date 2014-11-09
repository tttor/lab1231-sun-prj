#!/bin/sh
curr_dir="$(dirname "$0")"

#
num=$1
result_dir=/home/jogie/sun4/exp/overlap-segment/final-result/test-$num/
test_img_list_filepath=/home/jogie/sun4/exp/overlap-segment/meta/test$num.list

#
. "$curr_dir/set_dataset_param.sh"

#
exe=../../util/src-py/run_evaluator.py

#
python $exe $dataset_name $n_label $gt_img_dir $test_img_list_filepath $result_dir