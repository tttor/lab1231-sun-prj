#!/usr/bin/python

import os
import numpy as np
from sklearn.cross_validation import train_test_split

def chunks(l, n):
    """ Yield successive n-sized chunks from l.
        http://stackoverflow.com/questions/312443/how-do-you-split-a-list-into-evenly-sized-chunks-in-python
    """
    for i in xrange(0, len(l), n):
        yield l[i:i+n]

def main():
    dir_path = '/home/tor/sun3/dataset/pascal/VOC2012/VOCdevkit/VOC2012/SegmentationClass-csv/'
    filenames = os.listdir(dir_path)

    # D_tr and D_tr keep the index of filenames
    D_tr, D_te = train_test_split(range(len(filenames)),test_size=0.2)

    #
    max_chunk_size = 300
    D_tr_splitted = list(chunks(D_tr, max_chunk_size))

    list_dirpath = '/home/tor/sun4/xprmnt/ladicky-robustpn-voc2012/meta/'
    for i,d in enumerate(D_tr_splitted):
        list_filepath = list_dirpath + 'train' + str(i) + '.list'
        with open(list_filepath, "w") as list_file:
            for j,k in enumerate(d):
                list_file.write("%s\n" % filenames[k])

if __name__ == "__main__":
    main()
