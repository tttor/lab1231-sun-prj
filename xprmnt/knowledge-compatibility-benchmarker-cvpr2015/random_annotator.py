#!/usr/bin/python
'''
@obj Given an image, annotate each pixel with given classes uniformly at random
'''
from numpy import genfromtxt
import numpy as np
import sys

def annotate(filename, param):
    '''
    @brief

    \param filename a name of to-be-annotated file without any extension
    \param a dictionary that contains
            'gt_dir': directory of ground truth annotations
            'ratio': the ratio of randomized pixel, ratio \in [0.0, 1.0], from a coresponding ground truth
    '''
    # copy from the gt ann
    gt_ann_filepath = param['gt_dir'] + '/' + filename + '.csv'
    ann = genfromtxt(gt_ann_filepath, delimiter=',')

    # randomized
    n_rnd_idx = int( round(param['ratio']*ann.size) )
    rnd_idxes = np.random.random_integers(low=0, high=ann.shape[0]-1, size=(n_rnd_idx,2))# 2 is for 2D idx
    class_list = param['class_list']

    for i in range(rnd_idxes.shape[0]):
        row = rnd_idxes[i,:][0]
        col = rnd_idxes[i,:][1]

        ann[row][col] = np.random.random_integers(low=min(class_list), high=max(class_list))

    return ann


def main():
    assert len(sys.argv)==6, 'INSUFFICENT NUMBER OF ARGUMENTS'

    img_list_filepath = sys.argv[1]
    out_dir = sys.argv[2]

    #
    param = {}
    param['ratio'] = float(sys.argv[3])
    param['class_list'] = range(int(sys.argv[4]))
    param['gt_dir'] = sys.argv[5]

    with open(img_list_filepath) as f:
        img_list = f.readlines()
    img_list = [x.strip('\n') for x in img_list]

    for i, img in enumerate(img_list):
        print 'Annotating', str(i+1), 'of', str(len(img_list))
        ann = annotate(img, param)# dummy: input the merely img name instead of image filepath
        np.savetxt(out_dir+'/' + img + '.csv', ann, delimiter=",")

if __name__ == '__main__':
	main()