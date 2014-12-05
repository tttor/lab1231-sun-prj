#!/usr/bin/python
import numpy as np 
import csv

def get_segment_from_supfile(filepath):
    with open(filepath) as f:
        reader = csv.reader(f)
        segments = list(reader)

    for row in range(len(segments)):
        for col in range(len(segments[row])):
            segments[row][col] = int(segments[row][col])
    max_segment = np.max(segments)+1
    result = [[] for x in range(max_segment)]

    for row in range(len(segments)):
        for col in range(len(segments[row])):
            result[segments[row][col]].append( (row, col) )  

    return result

def get_region_from_regfile(filepath, clustered=False):
    '''
    \filepath path to a file from Region Descriptors, see http://www.robots.ox.ac.uk/~vgg/research/affine/descriptors.html#binaries
    \return a list of dictionary of {'param': <reg-location-param>, 'sift': <sift descriptor>}
    '''
    #
    with open(filepath) as f:
        content = f.readlines()
    content = [i.strip('\n') for i in content]

    #
    n_loc_param = 5
    n_sift_param = int(content[0])
    n_label_param = 0
    n_id_param = 0
    if clustered:
        n_label_param = 1
        n_id_param = 1
    n_region = int(content[1])
    
    content = content[2:]

    #
    region_list = []
    for line in content:
        region_param = line.split(' ')
        assert len(region_param)==(n_loc_param+n_sift_param+n_label_param+n_id_param), 'len(region_param)!=(n_loc_param+n_sift_param+n_label_param+n_id_param)'

        region = {}
        region['loc'] = [float(i) for i in region_param[0:n_loc_param]]
        region['sift'] = [float(i) for i in region_param[n_loc_param:-2]]

        if clustered:
            region['label'] = region_param[-2]
            region['id'] = region_param[-1]

        region_list.append(region)

    #
    assert len(region_list)==n_region, 'len(region_list)!=n_region'
    return region_list