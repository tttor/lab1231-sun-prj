#!/bin/sh
dir=/home/tor/xprmnt/knowledge-construction/relative-location-knowledge/prob-map-dirichlet-off.msrc/road

for f in "$dir"/*.pdf; do
    echo $f.png
    convert $f $f.png
done
