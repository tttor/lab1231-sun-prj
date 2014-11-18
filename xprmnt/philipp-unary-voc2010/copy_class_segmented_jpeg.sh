#!/bin/sh
curr_dir="$(dirname "$0")"
. "$curr_dir/set_dataset_param.sh"

target_dir=/home/tor/sun3/dataset/pascal/VOC2010/VOCdevkit/VOC2010/JPEGImages-ClassSegmented
rm -rf $target_dir
mkdir -p $target_dir

for i in `ls -1 $gt_img_dir | sed 's/\..*$//'`; 
  do cp -v $ori_img_dir$i* $target_dir; 
done

cd $target_dir
ls -F |grep -v / | wc -l
