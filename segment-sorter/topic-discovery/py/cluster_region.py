#!/usr/bin/python
'''
Input:
List of region-data file, e.g. .sift

Output:
KMeans model
'''

import cPickle
from sklearn.cluster import KMeans

def read_region(filepath):
    '''
    \filepath path to a file from Region Descriptors, see http://www.robots.ox.ac.uk/~vgg/research/affine/descriptors.html#binaries
    \return a list of dictionary of {'param': <reg-location-param>, 'sift': <sift descriptor>}
    '''
    #
    with open(filepath) as f:
        content = f.readlines()
    content = [i.strip('\n') for i in content]

    #
    n_location_param = 5
    n_sift_param = int(content[0])
    n_region = int(content[1])
    
    content = content[2:]

    #
    region_list = []
    for line in content:
        region_param = [float(i) for i in line.split(' ')]

        region = {}
        region['loc'] = region_param[0:n_location_param]
        region['sift'] = region_param[n_location_param:]
        assert len(region['sift'])==n_sift_param, 'len(region[])!=n_sift_param'
        
        region_list.append(region)

    #
    assert len(region_list)==n_region, 'len(region_list)!=n_region'

    return region_list

def read_region_from_list(list_filepath, region_data_dir):
    with open(list_filepath) as f:
        content = f.readlines()
    content = [i.strip('\n') for i in content]

    region_list = []
    for i, filename in enumerate(content):
        print('reading region data from: %s (%i of %i)' % (filename, i+1, len(content)))
        filepath = region_data_dir + '/' + filename 
        region_list = region_list + read_region(filepath)

    return region_list

def main():
    # Load
    list_filepath = '/media/tor/46DAF35ADAF344A9/tor/robotics/prj/011/xprmnt/segment-sorter/meta/msrc-21.sift.list'
    region_data_dir = '/home/tor/sun4/xprmnt/segment-sorter/region/msrc-21/sift'

    region_list = read_region_from_list(list_filepath, region_data_dir)

    # Cluster
    # TODO: preprocess?
    X = [ i['sift'] for i in region_list ]
    n_class = 21

    print('n_data= %i' % (len(region_list)))
    print('n_class= %i' % (n_class))

    kmeans = KMeans(init='k-means++', n_clusters=n_class, n_init=10, max_iter= 300)

    print('kmeans.fit()...')
    kmeans.fit(X)

    # Attach the cluster label to the region data
    for i, region in enumerate(region_list):
        region['label'] = kmeans.labels_[i]

    # Visualize
    # TODO: implement

    # Save the k-means model
    model_dir = '/home/tor/sun4/xprmnt/segment-sorter/kmeans-model'
    model_filename = 'test.kmeans'
    model_filepath = model_dir + '/' + model_filename
    with open(model_filepath, 'wb') as fid:
        cPickle.dump(kmeans, fid) 

if __name__ == '__main__':
    main()