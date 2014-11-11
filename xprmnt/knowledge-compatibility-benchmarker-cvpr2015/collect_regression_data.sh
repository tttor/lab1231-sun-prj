#!/bin/sh

#
ann_list_filename=ann_img_dummy.list
knowledge_dir=/home/tor/sun4/xprmnt/knowledge-compatibility-benchmarker-cvpr2015/knowledge
ann_list_filepath=/home/tor/sun4/xprmnt/knowledge-compatibility-benchmarker-cvpr2015/meta/split_voc2010_tor/$ann_list_filename
ann_dir=/home/tor/sun4/xprmnt/knowledge-compatibility-benchmarker-cvpr2015/annotation
out_dir=/home/tor/sun4/xprmnt/knowledge-compatibility-benchmarker-cvpr2015/regression-data

#
exe=collect_regression_data.py

python $exe $ann_list_filepath $knowledge_dir $ann_dir $out_dir
