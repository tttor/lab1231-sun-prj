#!/bin/bash
root_dir=/home/tor/xprmnt/knowledge-compatibility-benchmarker/regression-data
timestamp="$(date +'%Y%m%d.%H%M%S')"

annotator_id=philippunarymrf
dataset_name=voc2010
rawdata_timestamp=20150321.221555
data_dirpath=$root_dir/$annotator_id-$dataset_name-$rawdata_timestamp
exe=/home/tor/lab1231-sun-prj/knowledge-compatibility-benchmarker/preprocess.py

# input
declare -a fea_names=("cooccurrence_fea" "sceneprop_fea" "relloc_fea")
for fea_name in "${fea_names[@]}"
do   
   python $exe \
  	      $data_dirpath/input \
		  $fea_name
done

#output
out_name=ca
python  $exe \
		$data_dirpath/output \
		$out_name
