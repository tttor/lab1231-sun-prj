#!/bin/sh

#
num=$1
result_dir=/home/tor/sun4/xprmnt/ladicky-robustpn-mrsc/result/test-$num/
test_img_list_filepath=/home/tor/sun4/xprmnt/ladicky-robustpn-mrsc/meta/test$num.list

# Constant params across num
exe=../../ladicky-2009/build/main 

SLIC_region=20
SLIC_regularization=1000
SLIC_min_region=10

# Annotate
rm -rf $result_dir
mkdir -p $result_dir
$exe $dataset_name $n_label $ori_img_dir $test_img_list_filepath $result_dir $unary_philipp_dir $SLIC_region $SLIC_regularization $SLIC_min_region