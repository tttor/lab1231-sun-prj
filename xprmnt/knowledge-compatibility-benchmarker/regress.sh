#!/bin/bash
root_dir=/home/tor/xprmnt/knowledge-compatibility-benchmarker
timestamp="$(date +'%Y%m%d.%H%M%S')"
dataset_name=voc2010
annotator_id=philippunarymrf

regression_input_timestamp=20150314.224909
regression_data_dir=$root_dir/regression-data/$annotator_id-$dataset_name-$regression_input_timestamp

regression_output_dir=$root_dir/regression-result/$annotator_id-$dataset_name-$timestamp
mkdir -p $regression_output_dir

#
export PYTHONPATH=$PYTHONPATH:/home/tor/lab1231-sun-prj/util/src-py
exe=/home/tor/lab1231-sun-prj/knowledge-compatibility-benchmarker/regressing.py
python  $exe \
		$regression_data_dir \
		$regression_output_dir \

#
timestamp_end="$(date +'%Y%m%d.%H%M%S')"
echo $regression_data_dir > $regression_output_dir/regressing.meta
echo $dataset_name >> $regression_output_dir/regressing.meta 
echo $annotator_id >> $regression_output_dir/regressing.meta 
echo $timestamp >> $regression_output_dir/regressing.meta 
echo $timestamp_end >> $regression_output_dir/regressing.meta 
