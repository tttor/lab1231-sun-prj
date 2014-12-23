#!/bin/sh
src_dir=/home/tor/sun/ws/lab1231-sun-prj/philipp-unary/cc
unary_model_trainer_exe=$src_dir/build/src/unary_model_trainer
setting_filepath=$src_dir/src/settings.h

ori_img_dir=/home/tor/sun3/dataset/pascal/VOC2010/VOCdevkit/VOC2010/JPEGImages-ClassSegmented-scaled-0.25
gt_png_dir=/home/tor/sun3/dataset/pascal/VOC2010/VOCdevkit/VOC2010/SegmentationClass-scaled-0.25
train_list_filepath=/home/tor/sun3/dataset/pascal/VOC2010/meta/split_voc2010_philipp/Train.txt
model_out_dir=/home/tor/sun4/xprmnt/philipp-unary/unary-model

echo "Unary model training: begin"
rm -rf model_out_dir
mkdir -p $model_out_dir
cp $setting_filepath $model_out_dir

$unary_model_trainer_exe \
						$train_list_filepath \
						$ori_img_dir $gt_png_dir \
						$model_out_dir