#!/bin/sh
# This aims to collect the regression dataset $D={x_i,y_i}_{i=1}^{i=n}$.
# where: 
# x_i is the feature vector extracted from the knowledge and the predicted annotation
# y_i is the semantic segmentation accuracy (GA or CA) of a single predicted annotation
#
# Assumptions:
# > MRF-based annotation has been carried out, the result is put under $(ann_dir)/
# > Evaluation of the perfomance of semantic segmentation have been carried out

#
root_dir=/home/tor/xprmnt/knowledge-compatibility-benchmarker
timestamp="$(date +'%Y%m%d.%H%M%S')"

annotator_id=philippunarymrf
dataset_name=voc2010
ann_list_filename=ann_img_1928.list
ann_list_filepath=$root_dir/meta/$ann_list_filename

knowledge_dir=/home/tor/xprmnt/knowledge-construction
ann_dir=/home/tor/xprmnt/philipp-unary-mrf-voc2010/result/split_voc2010_philipp_Test_csv
eval_dir=/home/tor/xprmnt/philipp-unary-mrf-voc2010/eval
ori_img_dir=/home/tor/dataset/pascal/VOC2010/VOCdevkit/VOC2010/JPEGImages-ClassSegmented

regression_data_dir=$root_dir/regression-data/$annotator_id-$dataset_name-$timestamp
mkdir -p $regression_data_dir

#
export PYTHONPATH=$PYTHONPATH:/home/tor/lab1231-sun-prj/util/src-py
export PYTHONPATH=$PYTHONPATH:/home/tor/lab1231-sun-prj/knowledge-constructor
exe=/home/tor/lab1231-sun-prj/knowledge-compatibility-benchmarker/collect_regression_data.py
python $exe \
		$ann_list_filepath \
		$knowledge_dir \
		$ann_dir \
		$eval_dir \
		$regression_data_dir \
		$ori_img_dir

#
timestamp_end="$(date +'%Y%m%d.%H%M%S')"
echo $annotator_id > $regression_data_dir/regression_data.meta
echo $dataset_name >> $regression_data_dir/regression_data.meta
echo $ann_list_filepath >> $regression_data_dir/regression_data.meta
echo $eval_dir >> $regression_data_dir/regression_data.meta
echo $ann_dir >> $regression_data_dir/regression_data.meta
echo $knowledge_dir >> $regression_data_dir/regression_data.meta
echo $timestamp >> $regression_data_dir/regression_data.meta 
echo $timestamp_end >> $regression_data_dir/regression_data.meta 
