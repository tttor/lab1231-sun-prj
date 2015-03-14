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

# Extractor functions
def extract_relloc_fea(ann, knowledge):
    print 'WARN: does _not_ accomodate diriclet noise in offsets'
    ori_img_filepath = ann['ori_img_dir']+'/'+ann['filename']+ann['ori_img_ext']
    img = img_as_float(io.imread(ori_img_filepath))

    segmentation = rlk.get_segmentation(img)
    segment_list = rlk.get_segment_list(segmentation)
    class_list = knowledge.keys()

    # compute relative location votes
    init_vote = dict.fromkeys(range(len(segment_list)), None)
    for key in init_vote.iterkeys():
        init_vote[key] = dict.fromkeys(class_list, 0.0)

    vote_other = init_vote
    vote_self = init_vote

    for i, si in enumerate(segment_list):
        xi, yi = rlk.get_centroid(si)
        ci_hat = voc.class_id2name_map[ ann['ann'][xi,yi] ]
        if ci_hat in voc.ignored_class_name_list:
            continue

        for c in class_list:
            if c in voc.ignored_class_name_list:
                continue

            vote_other[i][c] = 0.0
            vote_self[i][c] = 0.0
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
                norm_offset = rlk.normalize_offset(offset, (200,200), ann['ann'].shape)
                
                if ci_hat==cj_hat:
                    vote_self[i][c]  = vote_self[i][c]  + ( alpha_j*knowledge[c][cj_hat][norm_offset[0],norm_offset[1]] )
                else:
                    vote_other[i][c] = vote_other[i][c] + ( alpha_j*knowledge[c][cj_hat][norm_offset[0]][norm_offset[1]] )

    # Compute the relloc features
    w_other = 1.0
    w_self = 1.0
    epsilon = 0.00001

    prob_list = []
    for s in range(len(segment_list)):
        for c in class_list:
            f_relloc = (w_other*math.log(vote_other[s][c]+epsilon)) + (w_self*math.log(vote_self[s][c]+epsilon))
            # f_relloc = (w_other*vote_other[s][c]) + (w_self*vote_self[s][c])
            prob_list.append(f_relloc)

    #
    fea = get_prob_list_representation(prob_list)
    return fea

def extract_cooccurrence_fea(ann, knowledge):
    numeric_classes = list( set(ann['ann'].flatten()) )    
    classes = [i for i in voc.translate(numeric_classes) if i not in voc.ignored_class_name_list]

    prob_list = [0.0]
    if len(classes) > 1:
        prob_list = [knowledge[i][j] for i,j in itertools.combinations(classes,2)]

    fea = get_prob_list_representation(prob_list)
    return fea

def extract_sceneprop_fea(ann, knowledge):
    '''
    Calculate the probabibility of each present object given the scene class.
    For now, the scene class is given by an Oracle
    '''
    scene_class = get_scene_class(ann['filename']).lower()

    numeric_classes = list( set(ann['ann'].flatten()) )    
    classes = [i for i in voc.translate(numeric_classes) if i not in voc.ignored_class_name_list]

    prob_list = [0.0]
    if len(classes) > 1:
        prob_list = [knowledge[scene_class][obj] if (obj in knowledge[scene_class].keys()) else 0.0 for obj in classes]
    
    fea = get_prob_list_representation(prob_list)
    return fea

# Helper functions
def get_scene_class(filename):
    '''
    This acts as an oracle that returns an absolutely correct scene class of a given image.
    \param filename 
    \return scene class
    '''
    scene_class_annotation_dir = '/home/tor/dataset/pascal/VOC2012/VOC2012/ScenePropertyAnnotations/cikupastar-20141111/ScenePropertyAnnotations-2'
    filepath = scene_class_annotation_dir + '/' + filename + '.xml'

    tree = etree.parse(filepath)
    root = tree.getroot()
    sub = root[0]
    scene_class = sub.attrib['name']

    return scene_class

def get_prob_list_representation(prob_list):
    assert len(prob_list)>0, 'len(prob_list)==0'

    prob_series = np.asarray(prob_list)
    rep = []

    # mean
    rep.append( np.mean(prob_series) )   

    # quartile    
    for i in [25,50,75]:
        rep.append( np.percentile(prob_series,i) )

    return rep

# def get_spatial(i, height, param):
#     top_height = int( round(param['ratio_top']*height) )
#     center_height = int( round(param['ratio_center']*height) )
 
#     top_center_th = top_height
#     center_bottom_th = top_height + center_height

#     if i < top_center_th:
#         return 'top'
#     elif i < center_bottom_th:
#         return 'center'
#     else:
#         return 'bottom'

# def extract_fea_s(ann, knowledge):
#     '''
#     @brief extract spatial compatibility score
#     For each annotated pixel (excluding 'void' and 'background', get the probability of such annotation given pixel spatial location.

#     \return the average probility of p_s from all annotated pixel.
#     '''
#     n_row = ann.shape[0]
#     n_col = ann.shape[1]

#     probs = []
#     for i in range(n_row):
#         for j in range(n_col):
#             ann_ij = class_name_map[ ann[i][j] ]
#             spatial = get_spatial(i, n_row, knowledge['param'])

#             if ann_ij!='background':
#                 prob = knowledge[ann_ij][spatial]
#                 probs.append(prob)

#     s = []
#     if len(probs) != 0:
#         s = [np.mean(probs), 
#              np.percentile(probs, 25), 
#              np.percentile(probs, 50), 
#              np.percentile(probs, 75)]
#     else:
#         s = [0.0] * 4
    
#     return s

# #def extract_fea_r(ann, knowledge):
# def extract_fea_r(filename, knowledge):
    
#     below_th = 20
#     around_th = 60
#     #philip_bbox_dir = '/home/ian-djakman/Documents/data/output/knowledge-compatibility-benchmarker/knowledge/philip_voc2010_boundingbox/boundingbox_data'
#     philip_bbox_dir = '/home/ian-djakman/Documents/data/output/knowledge-compatibility-benchmarker/knowledge/philip_voc2010_boundingbox/boundingbox_data_tor'

#     file_to_process = philip_bbox_dir + filename + '.json'
#     probs = []

#     with open(file_to_process, 'r') as f2p:
#         keyval = json.load(f2p)
#     bbox = keyval['pa_bbox']

#     bounding_box_amount = len(bbox)
#     for x in range (0, bounding_box_amount):
#         for y in range (x, bounding_box_amount):
#             if x == y :
#                 continue
#             else :
#                 obj1 = bbox[x]
#                 obj2 = bbox[y]
#                 obj1_classname = class_name_map[obj1[4]]
#                 obj2_classname = class_name_map[obj1[4]]
#                 prob_below = 0
#                 prob_beside = 0
#                 prob_around = 0
#                 #below
#                 if PositionSearcher.isBelow(obj1, obj2,below_th):
#                     prob_below = knowledge[obj1_classname]['is_below'][obj2_classname]

#                 #beside
#                 if PositionSearcher.isBeside(obj1, obj2):
#                     prob_beside = knowledge[obj1_classname]['is_beside'][obj2_classname]

#                 #inside
#                 if PositionSearcher.isInside(obj1, obj2):
#                     prob_around = knowledge[obj1_classname]['is_around'][obj2_classname]

#                 #if not inside, maybe it is around
#                 elif PositionSearcher.isAround(obj1, obj2, around_th):
#                     prob_around = knowledge[obj1_classname]['is_around'][obj2_classname]
                
#                 prob_highest = max(prob_below, prob_beside, prob_around)
#                 probs.append(prob_highest)
                
#                 '''
#                 print("bawah : " + str(prob_below))
#                 print("samping : " + str(prob_beside))
#                 print("sekitar : " + str(prob_around))
#                 print("yang paling tinggi : " + str(prob_highest))
#                 print(probs)
#                 '''
                
#     probs = np.asarray(probs)
#     r = []
#     if len(probs) != 0:
#         r = [np.mean(probs),
#              np.percentile(probs, 25), 
#              np.percentile(probs, 50), 
#              np.percentile(probs, 75)]
#     else:
#         r = [0.0] * 4

#     return r


# def extract_fea(ann_filepaths, knowledge_dir):
#     '''
#     fea = [c, s, r, p]
#     c: cooccurence
#     s: spatial layout
#     r: relative position
#     p: scene property
#     '''
#     #
#     knowledge = load_knowledge(knowledge_dir)

#     #
#     fea_list = []

#     for i, ann_filepath in enumerate(ann_filepaths):
#         print 'Extracting feature:', str(i+1), 'of', str(len(ann_filepaths))
#         ann = np.loadtxt(ann_filepath, delimiter=',')
#         dirpath_to_reduce = len(ann_filepath[:-16])
#         filename = (ann_filepath[dirpath_to_reduce:-4])

#         c = extract_fea_c(ann, knowledge['c'])
#         s = extract_fea_s(ann, knowledge['s'])
#         p = extract_fea_p(ann, knowledge['p'], ann_filepath.split('/')[-1][0:-4])
#         r = extract_fea_r(filename, knowledge['r'])
#         fea = r
#         fea_list.append(fea)
        
#     return fea_list

def main():
    print 'main()'
    
if __name__ == '__main__':
    main()
