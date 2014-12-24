#!/bin/sh

echo "setting dataset params...................................."

dataset_name=voc2010
echo $dataset_name

n_label=21 # 20 object classes + 1 background class
echo $n_label

ori_img_dir=/home/tor/sun3/dataset/pascal/VOC2010/VOCdevkit/VOC2010/JPEGImages-ClassSegmented-scaled-0.25
echo $ori_img_dir

# gt_csv_dir=/home/tor/sun3/dataset/pascal/VOC2012/VOCdevkit/VOC2010/SegmentationClass-csv-scaled-0.25/
# echo $gt_csv_dir

unary_philipp_dir=/home/tor/sun4/xprmnt/own-unary-voc2010-scaled/unary-compressed
echo $unary_philipp_dir

echo "-----------------------------------------------------------"
