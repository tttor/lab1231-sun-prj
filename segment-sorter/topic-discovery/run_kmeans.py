#!/usr/bin/python
import sys
import numpy as np
from sklearn.cluster import KMeans
from scipy.spatial.distance import cdist

def main(argv):
    n_cluster = int(argv[1])
    X_filepath = argv[2]
    X = np.genfromtxt(X_filepath, delimiter=',')

    kmeans = KMeans(n_clusters=n_cluster,n_jobs=-1)
    print('kmeans param: %s' % (kmeans.get_params))
    print('kmeans.fit()...')
    kmeans.fit(X)
    dist = sum(np.min(cdist(X, kmeans.cluster_centers_, 'euclidean'), axis=1)) / X.shape[0]

    out_dir = argv[3]
    dist_filepath = out_dir + '/' + 'meandistortion.' + str(n_cluster) + '.csv'
    np.savetxt(dist_filepath, np.asarray([dist]), delimiter=",")

if __name__ == '__main__':
    main(sys.argv)