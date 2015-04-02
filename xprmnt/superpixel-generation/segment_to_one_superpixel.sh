#!/bin/bash

home=/home/tor

img_dir=$home/dataset/pascal/VOC2010/VOCdevkit/VOC2010/JPEGImages-ClassSegmented 
img_list_filepath=$home/dataset/pascal/VOC2010/meta/split_voc2010_philipp/All_1928.txt 
img_extension=.jpg

out_dir=$home/xprmnt/superpixel-generation/voc2010/one-superpixel
mkdir -p $out_dir

exe=$home/lab1231-sun-prj/segmenter/src-py/segment_to_one_superpixel.py
python $exe  $img_dir \
			 $img_list_filepath \
			 $img_extension \
			 $out_dir
