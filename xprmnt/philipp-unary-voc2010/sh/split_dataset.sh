#!/bin/sh
curr_dir="$(dirname "$0")"
. "$curr_dir/set_dataset_param.sh"
root_dir=/home/tor/sun4/xprmnt/philipp-unary-voc2010

exe=../../util/src-py/split_dataset.py
src_dir=$ori_img_dir
list_dir=$root_dir/meta/

test_size=1927 #should be smaller than 1.0 or be an integer that is smaller than the number of samples
max_chunk_size=150

echo "Splitting ..."
rm -rf $list_dir
mkdir -p $list_dir
python $exe $src_dir $test_size $max_chunk_size $list_dir