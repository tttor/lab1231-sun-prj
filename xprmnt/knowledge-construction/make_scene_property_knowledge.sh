#!/bin/sh

root_dir=/home/tor/xprmnt/knowledge-construction/scene-property-knowledge
timestamp="$(date +'%Y%m%d.%H%M%S')"

ann_list_filepath=/home/tor/dataset/pascal/VOC2010/meta/split_voc2010_philipp/All_1928.txt
scene_prop_ann_dir=/home/tor/dataset/pascal/VOC2012/VOC2012/ScenePropertyAnnotations/cikupastar-20141111/ScenePropertyAnnotations-2
knowledge_dir=$root_dir

export PYTHONPATH=$PYTHONPATH:/home/tor/lab1231-sun-prj/util/src-py
exe=/home/tor/lab1231-sun-prj/knowledge-constructor/scene_property_knowledge.py

python $exe \
	   $ann_list_filepath \
	   $scene_prop_ann_dir \
	   $knowledge_dir
