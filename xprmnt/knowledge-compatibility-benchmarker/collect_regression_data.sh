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

ann_list_filename=ann_img.list
ann_list_filepath=$root_dir/meta/$ann_list_filename

knowledge_dir=/home/tor/xprmnt/knowledge-construction
ann_dir=/home/tor/xprmnt/philipp-unary-mrf-voc2010/result/split_voc2010_philipp_Test_csv
eval_dir=/home/tor/xprmnt/philipp-unary-mrf-voc2010/eval

regression_data_dir=$root_dir/regression-data/philippunarymrf_voc2010
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
		$regression_data_dir
