#!/usr/bin/python

import numpy as np
from lxml import etree
from os import listdir
from os.path import isfile, join

def construct_cooccurence_knowledge(meta_dir, out_dir):
    gt_ann_files = [ f for f in listdir(meta_dir) if isfile(join(meta_dir,f)) ]

    # extract the knowledge from the dataset
    knowledge = {}
    knowledge['person'] = {'person': 123, 'car': 456, 'dog': 333}
    knowledge['car'] = {'person': 456, 'car': 111, 'dog': 555}
    knowledge['dog'] = {'person': 333, 'car': 555, 'dog': 121}

    # check the knowledge validity
    # TODO

    # write the knowledge
    root = etree.Element("cooccurence_knowledge")
    root.set("dataset", "pascal_voc_2010")

    for key, val in knowledge.iteritems():
        sub = etree.SubElement(root, 'class')
        sub.set('name', key)

        subsub = etree.SubElement(sub, 'cooccur_with')
        for subkey, subval in val.iteritems():
            subsubsub = etree.SubElement(subsub, subkey)
            subsubsub.set('freq', str(subval))

    xml_filepath = out_dir + '/cooccurence_knowledge.xml'
    writer = etree.ElementTree(root)
    writer.write(xml_filepath, pretty_print=True)

def construct_knowledge(type, knowledge_dir):
    if type=='cooccurence':
        construct_cooccurence_knowledge(knowledge_dir)
    else:
        assert False, 'Unknown knowledge type'

def main():
    meta_knowledge_dir = '/home/tor/sun3/dataset/pascal/VOC2012/VOCdevkit/VOC2012/SegmentationClass-csv'
    knowledge_dir = '/home/tor/sun4/xprmnt/knowledge-compatibility-benchmarker-cvpr2015/knowledge'

    construct_cooccurence_knowledge(meta_knowledge_dir, knowledge_dir)

if __name__ == '__main__':
    main()