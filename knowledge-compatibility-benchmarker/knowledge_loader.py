#!/usr/bin/python

import numpy as np
from lxml import etree

def load_cooccurence_knowledge(filepath):
    knowledge = {}

    tree = etree.parse(filepath)
    root = tree.getroot()
    
    for sub in root:
        local_knowledge = {}
        for sub_2 in sub:
            for sub_3 in sub_2:
                local_knowledge[sub_3.tag] = float(sub_3.attrib['norm_freq'])

        knowledge[sub.attrib['name']] = local_knowledge

    return knowledge

def load_spatial_knowledge(filepath):
    knowledge = {}

    tree = etree.parse(filepath)
    root = tree.getroot()

    #
    for sub in root:
        local_knowledge = {}
        for sub_2 in sub:
                local_knowledge[sub_2.tag] = float(sub_2.attrib['norm_freq'])

        knowledge[sub.tag] = local_knowledge

    #
    uniform_ratio = float(1)/3# TODO should be read from the knowledge xml file
    param = {'ratio_top': uniform_ratio, 'ratio_center': uniform_ratio, 'ratio_bottom': uniform_ratio}
    knowledge['param'] = param

    return knowledge

def load_relativeposition_knowledge(filepath):
    knowledge = {}
    return knowledge

def load_sceneproperty_knowledge(filepath):
    knowledge = {}
    return knowledge

def load_knowledge(knowledge_dir):
    knowledge = {}
    knowledge['c'] = load_cooccurence_knowledge(knowledge_dir+'/cooccurence_knowledge.xml')
    knowledge['s'] = load_spatial_knowledge(knowledge_dir+'/spatial_knowledge.xml')
    knowledge['r'] = load_relativeposition_knowledge(knowledge_dir+'/relativeposition_knowledge.xml')
    knowledge['p'] = load_sceneproperty_knowledge(knowledge_dir+'/sceneproperty_knowledge.xml')

    return knowledge

def main():
    knowledge_dir = '/home/tor/sun4/xprmnt/knowledge-compatibility-benchmarker-cvpr2015/knowledge'
    knowledge = load_knowledge(knowledge_dir)

if __name__ == '__main__':
    main()