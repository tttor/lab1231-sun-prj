#!/bin/sh

root_dir=/home/tor/xprmnt/knowledge-construction/cooccurrence-knowledge
timestamp="$(date +'%Y%m%d.%H%M%S')"

ann_list_filepath=/home/tor/dataset/pascal/VOC2010/meta/split_voc2010_philipp/All_1928.txt
csv_ann_dir=/home/tor/dataset/pascal/VOC2010/VOCdevkit/VOC2010/SegmentationClass-csv
knowledge_dir=$root_dir

export PYTHONPATH=$PYTHONPATH:/home/tor/lab1231-sun-prj/util/src-py
exe=/home/tor/lab1231-sun-prj/knowledge-constructor/cooccurrence_knowledge.py

python $exe \
	   $ann_list_filepath \
	   $csv_ann_dir \
	   $knowledge_dir
