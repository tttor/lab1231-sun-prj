#!/bin/sh
# if [ "$#" -ne 1 ]; then
#     echo "Illegal number of parameters"
#     exit 1
# fi
curr_dir="$(dirname "$0")"

# Constant params across run number
exe=../../philipp-unary-mrf/build/main
root_dir=/home/tor/sun4/xprmnt/philipp-unary-msrc21
. "$curr_dir/set_dataset_param.sh"

# Varied params
# num=$1
result_dir=$root_dir/result/test
test_img_list_filepath=$root_dir/meta/split-msrc-philipp/split_msrc/Test.txt

# Annotate
rm -rf $result_dir
mkdir -p $result_dir
$exe $dataset_name $n_label $ori_img_dir $test_img_list_filepath $result_dir $unary_philipp_dir