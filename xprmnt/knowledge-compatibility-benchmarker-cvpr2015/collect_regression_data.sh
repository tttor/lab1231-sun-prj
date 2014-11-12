#!/bin/sh

#
root_dir=/home/tor/sun4/xprmnt/knowledge-compatibility-benchmarker-cvpr2015

ann_list_filename=ann_img.list
ann_list_filepath=$root_dir/meta/split-voc2010-tor/$ann_list_filename

knowledge_dir=$root_dir/knowledge
ann_dir=$root_dir/annotation/annotation-philippunary

out_dir=$root_dir/regression-data
regression_data_filename=voc2010_philippunary.csv
regression_data_filepath=$out_dir/$regression_data_filename

rm $regression_data_filepath

#
exe=collect_regression_data.py
python $exe $ann_list_filepath $knowledge_dir $ann_dir $regression_data_filepath