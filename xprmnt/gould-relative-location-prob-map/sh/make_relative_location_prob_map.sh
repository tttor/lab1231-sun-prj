#!/bin/sh
if [ "$#" -ne 1 ]; then
    echo "Illegal number of parameters"
    exit 1
fi

root_dir=/home/tor/xprmnt/gould-relative-location
timestamp="$(date +'%Y%m%d.%H%M%S')"

#
chosen_cprime=$1
img_list_filepath=/home/tor/sun3/dataset/msrc/meta/gtbmp_1_9_s.list
gt_csv_dir=/home/tor/sun3/dataset/msrc/unmix/GroundTruth-csv
img_dir=/home/tor/sun3/dataset/msrc/unmix/Images

prob_map_out_dir=$root_dir/prob-map.$timestamp

#
exe=../../../knowledge-constructor/make_gould_relative_location_prob_map.py
python $exe $chosen_cprime $img_list_filepath $gt_csv_dir $img_dir $prob_map_out_dir