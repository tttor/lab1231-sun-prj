#!/bin/bash
root_dir=/home/tor/xprmnt/knowledge-compatibility-benchmarker
timestamp="$(date +'%Y%m%d.%H%M%S')"
dataset_name=voc2010
annotator_id=philippunarymrf

regression_input_timestamp=20150314.224909
data_dirpath=$root_dir/regression-data/$annotator_id-$dataset_name-$regression_input_timestamp

result_dirpath=$root_dir/regression-result/$annotator_id-$dataset_name-$timestamp
meta_filepath=$result_dirpath/regression.meta
mkdir -p $result_dirpath

#
export PYTHONPATH=$PYTHONPATH:/home/tor/lab1231-sun-prj/util/src-py
exe=/home/tor/lab1231-sun-prj/knowledge-compatibility-benchmarker/regressing.py
python  $exe \
		$data_dirpath \
		$result_dirpath \
		$meta_filepath

#
echo $data_dirpath >> $meta_filepath
echo $dataset_name >> $meta_filepath 
echo $annotator_id >> $meta_filepath 
echo $timestamp >> $meta_filepath 
echo "$(date +'%Y%m%d.%H%M%S')" >> $meta_filepath 
