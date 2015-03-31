#!/usr/bin/python

import os
import sys
import numpy as np 
from skimage.util import img_as_float
from skimage import io

def main(argv):
    img_dir = argv[1]
    img_list_filepath = argv[2]
    img_extension = argv[3]
    out_dir = argv[4]

    # read the list
    with open(img_list_filepath) as f:
        img_ids = f.readlines()
    img_ids = [x.strip('\n') for x in img_ids]

    # 
    for i, img_id in enumerate(img_ids):
        print 'Segmenting ith=', i+1, 'of', len(img_ids)

        img_filepath = img_dir+'/'+img_id+img_extension
        img = img_as_float(io.imread(img_filepath))

        #
        shape = (img.shape[0], img.shape[1])
        segmentation = np.zeros(shape)

        segmentation_dir = out_dir+'/'+img_id
        if not os.path.exists(segmentation_dir):
            os.makedirs(segmentation_dir)

        segmentation_filepath = segmentation_dir+'/'+img_id+'-onesuperpixel'+'.sup'
        np.savetxt(segmentation_filepath, segmentation, delimiter=",")

        #
        n_pixel = img.shape[0]*img.shape[1]
        segmentation_onesegmentperrow = np.asarray(range(n_pixel))
        segmentation_onesegmentperrow = segmentation_onesegmentperrow.reshape((1,n_pixel))
        segmentation_filepath_2 = segmentation_dir+'/'+img_id+'-onesuperpixel'+'.sup2'
        np.savetxt(segmentation_filepath_2, segmentation_onesegmentperrow, delimiter=",")

if __name__ == '__main__':
    main(sys.argv)
