#!/bin/sh

echo "setting dataset params..."

dataset_name=voc2012
n_label=21 # 20 object classes + 1 background class
n_class=$n_label
ori_img_dir=/home/tor/sun3/dataset/pascal/VOC2012/VOCdevkit/VOC2012/JPEGImages-ClassSegmented
gt_img_dir=/home/tor/sun3/dataset/pascal/VOC2012/VOCdevkit/VOC2012/SegmentationClass
gt_csv_dir=/home/tor/sun3/dataset/pascal/VOC2012/VOCdevkit/VOC2012/SegmentationClass-csv
unary_philipp_dir=/home/tor/sun3/dataset/pascal/VOC2012/unary-philipp/voc2012_compressed