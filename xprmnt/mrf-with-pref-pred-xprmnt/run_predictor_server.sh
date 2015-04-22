#!/bin/bash
curr_dir="$(dirname "$0")"
home=/home/tor

#
. "$curr_dir/set_knowledge_dir.sh"
. "$curr_dir/set_regressor_filepath.sh"
. "$curr_dir/set_predictor_addr.sh"
. "$curr_dir/set_img_prop.sh"

# Annotate
exe=$home/lab1231-sun-prj/knowledge-compatibility-benchmarker/perf_predictor_server.py

python $exe $knowledge_dir \
	 		$regressor_filepath \
	 		$predictor_server_addr \
	 		$img_dir $img_extension
