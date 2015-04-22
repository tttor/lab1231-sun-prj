#!/usr/bin/python
import sys
import cPickle
import zmq
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

# in-memory variables
# http://stackoverflow.com/questions/6687660/keep-persistent-variables-in-memory-between-runs-of-python-script
# http://stackoverflow.com/questions/3047412/in-memory-database-in-python

def main(argv):
    assert len(argv)==6, 'INSUFFICENT NUMBER OF ARGUMENTS'
    knowledge_dir = argv[1]
    regressor_filepath = argv[2]
    port = argv[3]
    img_dir = argv[4]
    img_extension = argv[5]

    # Load the knowledge
    print 'Load the knowledge...'
    knowledge = {}
    knowledge['cooccurrence'] = kc.read(knowledge_dir+'/cooccurrence-knowledge/cooccurrence.xml')
    knowledge['sceneprop'] = ks.read(knowledge_dir+'/scene-property-knowledge/scene_property.xml')
    knowledge['relloc'] = kr.read_hdf5(knowledge_dir+'/relative-location-knowledge/relloc-hdf5/relloc.h5')
    
    #
    print 'Load the trained regressor...'
    regressor = None
    with open(regressor_filepath, 'rb') as fid:
        regressor = cPickle.load(fid)

    #
    print ('Open a connection at %s...'%port)
    context = zmq.Context()
    socket = context.socket(zmq.REP)
    socket.bind(port)

    while True:
        #  Wait for next request from client
        print 'Wait for the next request from client ...'
        request = socket.recv()# blocking effects
        # print("Received request: %s" % request)

        # Parse the msg
        ann_id = request.split(',')[0]
        ann_filepath = request.split(',')[1]

        # 
        ann = {}
        ann['ann'] = np.loadtxt(ann_filepath, delimiter=',')
        ann['filename'] = ann_id
        ann['ori_img_dir'] = img_dir
        ann['ori_img_ext'] = img_extension

        y_pred = predict(ann, knowledge, regressor)

        #  Send reply back to client
        respond = str(y_pred)
        socket.send(respond)

def predict(ann, knowledge, regressor):
    # Extract features
    print 'Extract features...'
    # print 'extract cooccurrence_fea'
    cooccurrence_fea = xtt.extract_cooccurrence_fea(ann,knowledge['cooccurrence'])
    # print 'extract sceneprop_fea'
    sceneprop_fea = xtt.extract_sceneprop_fea(ann,knowledge['sceneprop'])
    # print 'extract relloc_fea'
    relloc_fea = xtt.extract_relloc_fea(ann,knowledge['relloc'])

    X_te = cooccurrence_fea + sceneprop_fea + relloc_fea

    # TODO: preprocess X_te

    # Load and Do predict
    print 'Predict...'
    y_pred = regressor.predict(X_te)

    return y_pred[0]

if __name__ == '__main__':
	main(sys.argv)
