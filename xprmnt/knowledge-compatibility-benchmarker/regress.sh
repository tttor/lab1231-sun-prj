#!/bin/bash
if [ "$#" -ne 1 ]; then
    echo "Illegal number of parameters"
    exit 1
fi

root_dir=/home/tor/xprmnt/knowledge-compatibility-benchmarker
timestamp="$(date +'%Y%m%d.%H%M%S')"

#: main: (1) NuSVR and (2) GradientBoostingRegressor, 
#: secondary: GP (GaussianProcess), Lasso, DecisionTreeRegressionwithAdaBoost,
method=$1

dataset_name=voc2010
annotator_id=philippunarymrf
regression_input_timestamp=20150321.221555
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
		$meta_filepath \
		$method

#
echo $data_dirpath >> $meta_filepath
echo $dataset_name >> $meta_filepath 
echo $annotator_id >> $meta_filepath 
echo $timestamp >> $meta_filepath 
echo "$(date +'%Y%m%d.%H%M%S')" >> $meta_filepath 
