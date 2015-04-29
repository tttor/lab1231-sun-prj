#!/usr/bin/python
import sys
import numpy as np 
import json
import itertools
import math
from lxml import etree

from skimage.util import img_as_float
from skimage.segmentation import slic
from skimage import io
from skimage.segmentation import mark_boundaries

import pascal_voc_2012 as voc
import relative_location_knowledge as rlk
import feature_extractor_util as fxu

# Extractor functions
def extract_cooccurrence_fea(ann, knowledge):
    numeric_classes = list( set(ann['ann'].flatten()) )    
    classes = [i for i in voc.translate(numeric_classes) \
               if i not in voc.ignored_class_name_list]

    prob_list = [0.0]
    if len(classes) > 1:
        prob_list = [knowledge[i][j] for i,j \
                     in itertools.combinations(classes,2)]

    fea = fxu.get_prob_list_representation(prob_list)
    return fea


'''
@extract_sceneprop_fea(ann, knowledge)
Calculate the probabibility of each present object given the scene class.
For now, the scene class is given by an Oracle
'''
def extract_sceneprop_fea(ann, knowledge):
    scene_class = fxu.get_scene_class(ann['filename']).lower()

    numeric_classes = list( set(ann['ann'].flatten()) )    
    classes = [i for i in voc.translate(numeric_classes) \
               if i not in voc.ignored_class_name_list]

    prob_list = [0.0]
    if len(classes) > 1:
        prob_list = [knowledge[scene_class][obj] \
                     if (obj in knowledge[scene_class].keys()) \
                     else 0.0 for obj in classes]
    
    fea = fxu.get_prob_list_representation(prob_list)
    return fea

'''
@extract_relloc_fea(ann, knowledge)
'WARN: does _not_ accomodate diriclet noise in offsets'
'''
def extract_relloc_fea(ann, knowledge):
    # do (standard) segmentation
    ori_img_filepath = ann['ori_img_dir']+'/'+ann['filename']+ann['ori_img_ext']
    img = img_as_float(io.imread(ori_img_filepath))

    segmentation = rlk.get_segmentation(img)
    segment_list = rlk.get_segment_list(segmentation)
    class_list = knowledge.keys()

    # compute relative location votes
    init_vote = dict.fromkeys(range(len(segment_list)), None)
    for key in init_vote.iterkeys():
        init_vote[key] = dict.fromkeys(class_list, 0.0)

    vote_other =  vote_self = init_vote
    for i, si in enumerate(segment_list):
        xi, yi = rlk.get_centroid(si)
        ci_hat = voc.class_id2name_map[ ann['ann'][xi,yi] ]
        if ci_hat in voc.ignored_class_name_list:
            continue

        for c in class_list:
            if c in voc.ignored_class_name_list:
                continue

            vote_other[i][c] = vote_self[i][c] = 0.0
            for j in range(len(segment_list)):
                if j == i:
                    continue
                sj = segment_list[j]
                P_cjhat_sj = 1.0 # the probability of a superpixel sj has a class label cjhat
                alpha_j = P_cjhat_sj * len(sj)

                xj, yj = rlk.get_centroid(sj)
                cj_hat = voc.class_id2name_map[ ann['ann'][xj,yj] ]
                if cj_hat in voc.ignored_class_name_list:
                    continue
                
                offset = rlk.get_offset((xi,yi),(xj,yj))
                norm_offset = rlk.normalize_offset(offset, (200,200), \
                                                   ann['ann'].shape)

                prob_map = knowledge[c][cj_hat]
                idx_x, idx_y = rlk.get_prob_map_idx(norm_offset, prob_map.shape)

                if ci_hat==cj_hat:
                    vote_self[i][c]  = vote_self[i][c] \
                                       + (alpha_j*prob_map[idx_x][idx_y])
                else:
                    vote_other[i][c] = vote_other[i][c] \
                                       + (alpha_j*prob_map[idx_x][idx_y])

    # Compute the relloc features
    w_other = 1.0; w_self = 1.0; epsilon = 0.00001

    prob_list = []
    for s in range(len(segment_list)):
        for c in class_list:
            f_relloc = (w_other*math.log(vote_other[s][c]+epsilon)) \
                       + (w_self*math.log(vote_self[s][c]+epsilon))
            prob_list.append(f_relloc)

    # Get the representation of pseudo prob map
    fea = fxu.get_prob_list_representation(prob_list)
    return fea

def main():
    print 'main()'
    
if __name__ == '__main__':
    main()
