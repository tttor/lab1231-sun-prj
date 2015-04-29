#!/usr/bin/python

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
