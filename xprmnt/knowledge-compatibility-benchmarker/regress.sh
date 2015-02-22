#!/bin/sh
timestamp="$(date +'%Y%m%d.%H%M%S')"

regression_data_dir=/home/tor/xprmnt/knowledge-compatibility-benchmarker/regression-data/philippunarymrf_voc2010
regression_output_dir=/home/tor/xprmnt/knowledge-compatibility-benchmarker/regression-output/$timestamp

mkdir -p $regression_output_dir

#
export PYTHONPATH=$PYTHONPATH:/home/tor/lab1231-sun-prj/util/src-py
exe=/home/tor/lab1231-sun-prj/knowledge-compatibility-benchmarker/regressing.py
python  $exe \
		$regression_data_dir \
		$regression_output_dir \
