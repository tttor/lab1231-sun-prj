#!/usr/bin/python
import sys
import numpy as np
from sklearn.cluster import KMeans
from scipy.spatial.distance import cdist
from sklearn.utils import shuffle

def main(argv):
    assert len(argv)==5, 'INSUFFICIENT NUMBER OF ARGUMENTS!'
    n_cluster = int(argv[1])
    used_sample_ratio = float(argv[2])
    X_filepath = argv[3]
    out_dir = argv[4]

    raw_X = np.genfromtxt(X_filepath, delimiter=',')
    n_used_sample = int(used_sample_ratio*len(raw_X))
    X = shuffle(raw_X,n_samples=n_used_sample)
    print('Using %i out of %i samples'%(len(X),len(raw_X)))

    kmeans = KMeans(n_clusters=n_cluster,n_jobs=-1)
    print('kmeans param: %s' % (kmeans.get_params))
    print('kmeans.fit()...')
    kmeans.fit(X)
    dist = sum(np.min(cdist(X, kmeans.cluster_centers_, 'euclidean'), axis=1)) / X.shape[0]

    dist_filepath = out_dir + '/' + 'meandistortion.' + str(n_cluster) + '.csv'
    np.savetxt(dist_filepath, np.asarray([dist]), delimiter=",")

if __name__ == '__main__':
    main(sys.argv)