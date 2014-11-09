#!/bin/sh

#
num=$1

test_img_list_filepath=/home/jogie/sun4/exp/overlap-segment/meta/test$num.list
param_superpixel=/home/jogie/sun4/exp/overlap-segment/meta/segment-param.list
# Constant params across num
exe=../../segmenter/src-py/segment.py

ori_img_dir=/home/jogie/sun3/dataset/msrc/unmix/Images/
superpixel_dir=/home/jogie/sun4/exp/overlap-segment/superpixel/
# Annotate
python $exe $test_img_list_filepath $param_superpixel $ori_img_dir $superpixel_dir