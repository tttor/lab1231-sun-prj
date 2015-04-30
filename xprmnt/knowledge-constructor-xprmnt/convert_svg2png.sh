#!/bin/bash
dir=/home/tor/Dropbox/robotics/prj/011/wrt/tor/kc-benchmarker/s2is-usings2is/fig/knowledge/scene-prop

for f in "$dir"/*.svg; do
	s2=`echo $f| cut -d'/' -f 15`
	s3=`echo $s2| cut -d'.' -f 1`
    echo $s3.png
    convert $f $dir/$s3.png
done