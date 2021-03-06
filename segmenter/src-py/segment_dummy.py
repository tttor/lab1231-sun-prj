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
    n_segment = int(argv[4])
    out_dir = argv[5]

    # read the list
    with open(img_list_filepath) as f:
        img_ids = f.readlines()
    img_ids = [x.strip('\n') for x in img_ids]

    # 
    for i, img_id in enumerate(img_ids):
        print 'Segmenting ith=', i+1, 'of', len(img_ids)

        img_filepath = img_dir+'/'+img_id+img_extension
        img = img_as_float(io.imread(img_filepath))

        segmentation_dir = out_dir+'/'+img_id
        if not os.path.exists(segmentation_dir):
            os.makedirs(segmentation_dir)

        # #
        # shape = (img.shape[0], img.shape[1])
        # segmentation = np.zeros(shape)

        # segmentation_filepath = segmentation_dir+'/'+img_id+'-onesuperpixel'+'.sup'
        # np.savetxt(segmentation_filepath, segmentation, delimiter=",")

        #
        n_pixel = img.shape[0]*img.shape[1]
        n_max_pixel_per_segment = n_pixel / n_segment
        # segmentation_onesegmentperrow = np.asarray(range(n_pixel))
        # segmentation_onesegmentperrow = segmentation_onesegmentperrow.reshape((1,n_pixel))

        # Notice that the _actual_ number of segments (lines in sup2) 
        # is either n_segment or (n_segment+1)
        str_buf = ''
        for p in range(n_pixel):
            str_buf += str(p)
            if (p+1)%n_max_pixel_per_segment == 0:
                str_buf += '\n'
            else:
                str_buf += ','

        segmentation_filepath_2 = segmentation_dir+'/'+img_id \
                                  + '-' + str(n_segment) + 'dummyseg'+'.sup2'
        # np.savetxt(segmentation_filepath_2, segmentation_onesegmentperrow, delimiter=",")
        with open(segmentation_filepath_2, 'w') as f:
            f.write(str_buf)

if __name__ == '__main__':
    main(sys.argv)
