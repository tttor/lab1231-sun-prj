#!/bin/sh
if [ "$#" -ne 2 ]; then
    echo "Illegal number of parameters"
    exit 1
fi

root_dir=/home/tor/xprmnt/knowledge-construction/relative-location-knowledge
timestamp="$(date +'%Y%m%d.%H%M%S')"

#
chosen_cprime=$1
dirichlet_noise=False
dataset_name=$2

echo "dataset_name= "$dataset_name
if [ "$dataset_name" = "msrc" ]
then
    img_list_filepath=/home/tor/dataset/msrc/meta/591.list
	gt_csv_dir=/home/tor/dataset/msrc/unmix/GroundTruth-csv
	img_dir=/home/tor/dataset/msrc/unmix/Images
elif [ "$dataset_name" = "voc" ]
then
	img_list_filepath=/home/tor/dataset/pascal/VOC2010/meta/split_voc2010_philipp/All_1928.txt
	gt_csv_dir=/home/tor/dataset/pascal/VOC2010/VOCdevkit/VOC2010/SegmentationClass-csv
	img_dir=/home/tor/dataset/pascal/VOC2010/VOCdevkit/VOC2010/JPEGImages
else
    echo "ERR: Unknown dataset_name!"
    exit 1
fi

prob_map_out_id=prob-map-dirichlet-off.$dataset_name.$timestamp
prob_map_out_dir=$root_dir/$prob_map_out_id

#
export PYTHONPATH=$PYTHONPATH:/home/tor/lab1231-sun-prj/util/src-py
exe=/home/tor/lab1231-sun-prj/knowledge-constructor/relative_location_knowledge.py
python  $exe \
		$chosen_cprime $dirichlet_noise \
		$img_list_filepath $gt_csv_dir $img_dir \
		$prob_map_out_dir \
		$dataset_name

#
pdf_map_dir=$prob_map_out_dir/$chosen_cprime
for f in "$pdf_map_dir"/*.pdf; do
    echo $f.png
    convert $f $f.png
    # convert -crop 340x340+125+45 $f.png $f.cropped.png 
done
