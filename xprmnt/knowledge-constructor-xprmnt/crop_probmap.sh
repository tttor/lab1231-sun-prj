#!/bin/sh
dir=/home/tor/xprmnt/knowledge-construction/relative-location-knowledge/prob-map-dirichlet-off/tvmonitor

for f in "$dir"/*.png; do
    echo $f
    convert -crop 340x340+125+45 $f $f
done