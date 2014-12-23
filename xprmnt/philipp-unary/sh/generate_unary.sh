#!/bin/sh
exe_dir=/home/tor/sun/ws/lab1231-sun-prj/philipp-unary/cc/build/src
unary_generator_exe=$exe_dir/unary_generator

ori_img_dir=/home/tor/sun3/dataset/pascal/VOC2010/VOCdevkit/VOC2010/JPEGImages-ClassSegmented-scaled-0.25
test_list_filepath=/home/tor/sun3/dataset/pascal/VOC2010/meta/split_voc2010_philipp/Test.txt

unary_model_dir=/home/tor/sun4/xprmnt/philipp-unary/unary-model
unary_out_dir=/home/tor/sun4/xprmnt/philipp-unary/unary

rm -rf $unary_out_dir
mkdir -p $unary_out_dir

echo "START TO GENERATE UNARIES FOR TEST IMAGES"
$unary_generator_exe \
			$unary_model_dir \
			$test_list_filepath \
			$ori_img_dir \
			$unary_out_dir