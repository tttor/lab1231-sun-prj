#!/bin/bash
root_dir=/home/tor/xprmnt/knowledge-compatibility-benchmarker/regression-data
timestamp="$(date +'%Y%m%d.%H%M%S')"

annotator_id=philippunarymrf
dataset_name=voc2010
rawdata_timestamp=20150314.224909

regression_data_dirpath=$root_dir/$annotator_id-$dataset_name-$rawdata_timestamp/input
fea_name=cooccurrence_fea
# fea_name=sceneprop_fea
# fea_name=relloc_fea

declare -a fea_names=("cooccurrence_fea" "sceneprop_fea" "relloc_fea")

for fea_name in "${fea_names[@]}"
do
   exe=/home/tor/lab1231-sun-prj/knowledge-compatibility-benchmarker/preprocess.py
   python $exe \
  	      $regression_data_dirpath \
		  $fea_name
done
