#!/bin/sh
if [ "$#" -ne 1 ]; then
    echo "Illegal number of parameters"
    exit 1
fi

root_dir=/home/tor/xprmnt/gould-relative-location-prob-map
timestamp="$(date +'%Y%m%d.%H%M%S')"

#
chosen_cprime=$1
dirichlet_noise=False
img_list_filepath=/home/tor/dataset/msrc/meta/gtbmp_1_9_s.list
gt_csv_dir=/home/tor/dataset/msrc/unmix/GroundTruth-csv
img_dir=/home/tor/dataset/msrc/unmix/Images

prob_map_out_dir=$root_dir/prob-map.$timestamp

#
export PYTHONPATH=$PYTHONPATH:/home/tor/ws/lab1231-sun-prj/util/src-py
exe=../../../knowledge-constructor/make_gould_relative_location_prob_map.py
python  $exe \
		$chosen_cprime $dirichlet_noise \
		$img_list_filepath $gt_csv_dir $img_dir \
		$prob_map_out_dir
