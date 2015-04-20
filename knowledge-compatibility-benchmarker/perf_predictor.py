#!/usr/bin/python
import cPickle
import numpy as np
from pandas import read_hdf

import feature_extractor as xtt
import cooccurrence_knowledge as kc
import scene_property_knowledge as ks
import relative_location_knowledge as kr

# faster read and write file format in python
# http://stackoverflow.com/questions/22941147/fastest-file-format-for-read-write-operations-with-pandas-and-or-numpy
# http://stackoverflow.com/questions/21607974/pytables-installation-failed

# communicate between multiple running python programs
# http://stackoverflow.com/questions/16213235/communication-between-two-python-scripts
# http://zeromq.org/bindings:python

def predict(ann_id, ann_filepath, knowledge_dir, ml_model_filepath,
            ori_img_dir, ori_img_ext, ypred_filepath, tmp_dir):
    # Extract features
    print 'Extract features...'
    cooccurrence_knowledge = kc.read(knowledge_dir+'/cooccurrence-knowledge/cooccurrence.xml')
    sceneprop_knowledge = ks.read(knowledge_dir+'/scene-property-knowledge/scene_property.xml')
    relloc_knowledge = kr.read_hdf5(knowledge_dir+'/relative-location-knowledge/relloc-hdf5/relloc.h5')

    ann = {}
    ann['ann'] = np.loadtxt(ann_filepath, delimiter=',')
    ann['filename'] = ann_id
    ann['ori_img_dir'] = ori_img_dir
    ann['ori_img_ext'] = ori_img_ext
    
    # print 'extract cooccurrence_fea';
    cooccurrence_fea = xtt.extract_cooccurrence_fea(ann,cooccurrence_knowledge)
    # print 'extract sceneprop_fea';
    sceneprop_fea = xtt.extract_sceneprop_fea(ann,sceneprop_knowledge)
    # print 'extract relloc_fea';
    relloc_fea = xtt.extract_relloc_fea(ann,relloc_knowledge)

    X_te = cooccurrence_fea + sceneprop_fea + relloc_fea
    # TODO: preprocess X_te

    # Load and Do predict
    print 'Predict...'
    with open(ml_model_filepath, 'rb') as fid:
        estimator = cPickle.load(fid)
    y_pred = estimator.predict(X_te)

    with open(ypred_filepath, 'wb') as fo:
        fo.write(str(y_pred[0]))

def main():
	pass

if __name__ == '__main__':
	main()