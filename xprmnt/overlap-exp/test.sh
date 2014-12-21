#!/bin/sh

#
num=$1
result_dir=/home/jogie/sun4/exp/overlap-segment/ann-result/test-$num/
final_result_dir=/home/jogie/sun4/exp/overlap-segment/final-result/test-$num/
test_img_list_filepath=/home/jogie/sun4/exp/overlap-segment/meta/test$num.list
param_superpixel=/home/jogie/sun4/exp/overlap-segment/meta/segment-param-test.list
# Constant params across num
exe=../../overlap-exp/build/main 

dataset_name=msrc
n_label=21
ori_img_dir=/home/jogie/sun3/dataset/msrc/unmix/Images/
gt_img_dir=/home/jogie/sun3/dataset/msrc/unmix/Newgt/
unary_philipp_dir=/home/jogie/sun3/dataset/msrc/unary_philipp/msrc_compressed/
superpixel_dir=/home/jogie/sun4/exp/overlap-segment/superpixel/
# Annotate
rm -rf $result_dir
mkdir -p $result_dir
$exe $dataset_name $n_label $ori_img_dir $test_img_list_filepath $result_dir $unary_philipp_dir $superpixel_dir $param_superpixel $final_result_dir