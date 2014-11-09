#!/bin/sh

echo "setting dataset params..."

dataset_name=voc2010
n_label=21 # 20 object classes + 1 background class
ori_img_dir=/home/tor/sun3/dataset/pascal/VOC2010/VOCdevkit/VOC2010/JPEGImages-ClassSegmented/
gt_img_dir=/home/tor/sun3/dataset/pascal/VOC2012/VOCdevkit/VOC2010/SegmentationClass-csv/
unary_philipp_dir=/home/tor/sun3/dataset/pascal/VOC2010/unary-philipp/voc2010_compressed/