#/bin/bash

image_dir=/home/tor/sun3/dataset/stanford-background-dataset/iccv09Data/images
label_dir=/home/tor/sun3/dataset/stanford-background-dataset/iccv09Data/labels
bin_dir=/home/tor/robotics/prj/011/ws/lab1231-sun-prj/cpp-ws/build/src/main

out_dir=/home/tor/sun3/dataset/stanford-background-dataset/iccv09Data/ground-truth-seg-images
rm -rf $out_dir
mkdir $out_dir

for entry in $image_dir/*
do
    echo creating segmented image of $entry
    $bin_dir/create_ground_truth_image_of_stanford_background_dataset $entry $label_dir $out_dir
done
