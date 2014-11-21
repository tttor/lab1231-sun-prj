#!/bin/sh

#
num=$1
result_dir=/home/jogie/sun4/exp/ladicky-2009-mrsc/ann-result/test-$num/
test_img_list_filepath=/home/jogie/sun4/exp/ladicky-2009-mrsc/meta/test$num.list

# Constant params across num
exe=../../ladicky-2009/build/main 

SLIC_region=25
SLIC_regularization=70
SLIC_min_region=15
dataset_name=msrc
n_label=21
ori_img_dir=/home/jogie/sun3/dataset/msrc/unmix/Images/
gt_img_dir=/home/jogie/sun3/dataset/msrc/unmix/GroundTruth/
unary_philipp_dir=/home/jogie/sun3/dataset/msrc/unary_philipp/msrc_compressed/
# Annotate
rm -rf $result_dir
mkdir -p $result_dir
$exe $dataset_name $n_label $ori_img_dir $test_img_list_filepath $result_dir $unary_philipp_dir $SLIC_region $SLIC_regularization $SLIC_min_region