#!/usr/bin/python

import numpy as np 
from knowledge_loader import load_knowledge
from pascal_voc_2012 import translate
from pascal_voc_2012 import class_name_map

def get_spatial(i, height, param):
    top_height = int( round(param['ratio_top']*height) )
    center_height = int( round(param['ratio_center']*height) )
 
    top_center_th = top_height
    center_bottom_th = top_height + center_height

    if i < top_center_th:
        return 'top'
    elif i < center_bottom_th:
        return 'center'
    else:
        return 'bottom'

def extract_fea_c(ann, knowledge):
    numeric_classes = list( set(ann.flatten()) )
    raw_classes = translate(numeric_classes)
    classes = [i for i in raw_classes if i!='background' and i!='void']

    probs = [knowledge[classes[i]][classes[j]] for i in range(len(classes)-1) for j in range(i+1,len(classes))]

    c = 0.0
    if len(probs) != 0:
        c = sum(probs)/len(probs)

    return c

def extract_fea_s(ann, knowledge):
    '''
    @brief extract spatial compatibility score
    For each annotated pixel (excluding 'void' and 'background', get the probability of such annotation given pixel spatial location.

    \return the average probility of p_s from all annotated pixel.
    '''
    n_row = ann.shape[0]
    n_col = ann.shape[1]

    probs = []
    for i in range(n_row):
        for j in range(n_col):
            ann_ij = class_name_map[ ann[i][j] ]
            spatial = get_spatial(i, n_row, knowledge['param'])

            if ann_ij!='background':
                prob = knowledge[ann_ij][spatial]
                probs.append(prob)

    s = 0.0
    if len(probs) != 0:
        s = sum(probs)/len(probs)

    return s

def extract_fea_r(ann, knowledge):
    return 0.0

def extract_fea_p(ann, knowledge):
    return 0.0

def extract_fea(ann_filepaths, knowledge_dir):
    '''
    fea = [c, s, r, p]
    c: cooccurence
    s: spatial layout
    r: relative position
    p: scene property
    '''
    #
    knowledge = load_knowledge(knowledge_dir)

    #
    fea_list = []

    for i, ann_filepath in enumerate(ann_filepaths):
        print 'Extracting feature:', str(i+1), 'of', str(len(ann_filepaths))

        ann = np.loadtxt(ann_filepath, delimiter=',')

        c = extract_fea_c(ann, knowledge['c'])
        s = extract_fea_s(ann, knowledge['s'])
        r = extract_fea_r(ann, knowledge['r'])
        p = extract_fea_p(ann, knowledge['p'])

        fea = [c, s, r, p]
        fea_list.append(fea)

    return fea_list

def main():
    print 'main()'
    
if __name__ == '__main__':
    main()