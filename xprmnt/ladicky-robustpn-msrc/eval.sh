#!/bin/sh
curr_dir="$(dirname "$0")"

#
num=$1
result_dir=/home/tor/sun4/xprmnt/ladicky-robustpn-mrsc/result/test-$num/
test_img_list_filepath=/home/tor/sun4/xprmnt/ladicky-robustpn-mrsc/meta/test$num.list

#
. "$curr_dir/set_dataset_param.sh"

#
exe=../../util/src-py/run_evaluator.py

#
python $exe $dataset_name $n_label $gt_img_dir $test_img_list_filepath $result_dir