#!/usr/bin/python

import os
import sys
import numpy as np
from sklearn.cross_validation import train_test_split

def chunks(l, n):
    """ Yield successive n-sized chunks from l.
        http://stackoverflow.com/questions/312443/how-do-you-split-a-list-into-evenly-sized-chunks-in-python
    """
    for i in xrange(0, len(l), n):
        yield l[i:i+n]

def main():
    if len(sys.argv) != 5:
        assert False, "INSUFFICIENT ARGUMENTS!"

    src_dir = sys.argv[1]
    filenames = os.listdir(src_dir)

    # D_tr and D_tr keep the index of filenames
    if '.' in sys.argv[2]:
        test_size = float(sys.argv[2])
    else:
        test_size = int(sys.argv[2])

    D_tr, D_te = train_test_split(range(len(filenames)),test_size=test_size)
    # print 'len(D_tr)=', len(D_tr)
    # print 'len(D_te)=', len(D_te)

    #
    max_chunk_size = int(sys.argv[3])
    D_tr_splitted = list(chunks(D_tr, max_chunk_size))
    D_te_splitted = list(chunks(D_te, max_chunk_size))

    list_dir = sys.argv[4]
    for i,d in enumerate(D_tr_splitted):
        list_filepath = list_dir + 'train' + str(i+1) + '.list'
        with open(list_filepath, "w") as list_file:
            for j,k in enumerate(d):
                list_file.write("%s\n" % filenames[k])

    for i,d in enumerate(D_te_splitted):
        list_filepath = list_dir + 'test' + str(i+1) + '.list'
        with open(list_filepath, "w") as list_file:
            for j,k in enumerate(d):
                list_file.write("%s\n" % filenames[k])

if __name__ == "__main__":
    main()
