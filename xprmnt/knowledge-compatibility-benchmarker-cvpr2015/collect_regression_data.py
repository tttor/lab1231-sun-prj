#!/usr/bin/python

import sys
import numpy as np

from feature_extractor import extract_fea
from evaluator_tor import get_average_acc_over_clasess_from_file

def main():
    assert len(sys.argv)==5, 'INSUFFICENT NUMBER OF ARGUMENTS'

    # Extract features
    ann_list_filepath = sys.argv[1]
    knowledge_dir = sys.argv[2]
    ann_dir = sys.argv[3]

    with open(ann_list_filepath) as f:
        target_list = f.readlines()
    target_list = [x.strip('\n') for x in target_list]
    ann_filepaths = [ann_dir+'/'+target+'.csv' for target in target_list]

    fea_list = extract_fea(ann_filepaths, knowledge_dir)

    # Extract regression outputs
    print('Extract regression outputs ...')
    evl_filepaths = [ann_dir+'/'+target+'.evl' for target in target_list]
    out_list = [get_average_acc_over_clasess_from_file(filepath) for filepath in evl_filepaths]

    # Write the D = {(X,y)}
    assert len(out_list)==len(fea_list), 'len(out_list)!=len(fea_list)'

    data = [ fea_list[i]+[out_list[i]] for i in range(len(out_list)) ]
    data = np.asarray(data)

    out_filepath = sys.argv[4]
    np.savetxt(out_filepath, data, delimiter=",")

if __name__ == '__main__':
    main()