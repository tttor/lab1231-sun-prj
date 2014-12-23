#!/bin/sh
exe_dir=/home/tor/sun/ws/lab1231-sun-prj/philipp-unary/cc/build/src
unary_generator_exe=$exe_dir/unary_generator

ori_img_dir=/home/tor/sun3/dataset/pascal/VOC2010/VOCdevkit/VOC2010/JPEGImages-ClassSegmented
test_list_filepath=/home/tor/sun3/dataset/pascal/VOC2010/meta/split_voc2010_philipp/Test.txt

#create google unary
echo "START TO GENERATE UNARIES FOR TEST IMAGES"
$predict_bin \
			$improved_param \
			$test_list_filepath \
			$ori_img_dir \
			$google_test_unary_path