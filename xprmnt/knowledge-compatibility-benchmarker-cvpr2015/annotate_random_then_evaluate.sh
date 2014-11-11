#!/bin/sh
if [ "$#" -ne 2 ]; then
    echo "Illegal number of parameters"
    exit 1
fi
curr_dir="$(dirname "$0")"
. "$curr_dir/set_dataset_param.sh"

ratio=$1
batch=$2

root_dir=/home/tor/sun4/xprmnt/knowledge-compatibility-benchmarker-cvpr2015

img_list_filepath=$root_dir/meta/split-voc2012-tor/target_img.list
out_dir=$root_dir/annotation/annotation-random/ratio-$ratio-batch-$batch

# Annotate
rm -rf $out_dir
mkdir -p $out_dir

exe=random_annotator.py
python $exe $img_list_filepath $out_dir $ratio $n_class $gt_csv_dir

# Evaluate
evaluator=../../util/src-py/evaluator_tor.py 

pred_dir=$out_dir
python $evaluator $img_list_filepath $pred_dir $gt_csv_dir