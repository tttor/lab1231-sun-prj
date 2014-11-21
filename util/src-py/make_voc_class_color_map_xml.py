#!/usr/bin/python

from lxml import etree
import cv2
import numpy as np
from pascal_voc_2012 import class_name_map

def main():
    src_dir = '/home/tor/sun3/dataset/pascal/VOC2010/VOCdevkit/VOC2010/class-color'
    n_class = 21

    class_color_map = {}
    for i in range(n_class):
	   # Read class image
       img_filepath = src_dir + '/class_' + str(i) + ".png"
       img = cv2.imread(img_filepath)

       x = y = 0# only need to access a single pixel as all pixels have the same color
       bgr = (img.item(y,x,0),img.item(y,x,1),img.item(y,x,2))
       class_color_map[i] = bgr

    # Write XML
    root = etree.Element("class_color_map")
    root.set("dataset_name", "pascal_voc_2010")

    for key, val in class_color_map.iteritems():
        sub = etree.SubElement(root, "class")
        sub.set('num', str(key))
        sub.set('name', class_name_map[key])

        subsub = etree.SubElement(sub, "color")
        subsub.set("b", str(val[0]))
        subsub.set("g", str(val[1]))
        subsub.set("r", str(val[2]))
        
    out_dir = src_dir
    xml_filepath = out_dir + '/class_color_map.xml'
    writer = etree.ElementTree(root)
    writer.write(xml_filepath, pretty_print=True)

if __name__ == "__main__":
	main();