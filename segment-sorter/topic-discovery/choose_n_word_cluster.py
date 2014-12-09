#!/usr/bin/python
'''
If K is not specified by the problem's context, the optimal number of clusters can
be estimated using a technique called the elbow method
'''
import sys
import numpy as np
from sklearn.cluster import KMeans
from scipy.spatial.distance import cdist
import matplotlib.pyplot as plt
from matplotlib.backends.backend_pdf import PdfPages

def elbow_method(K, X, param):
    meandistortions = []
    for i, k in enumerate(K):
        print('Clustering with k=%i (%i/%i)' % (k,i+1,len(K)))
        kmeans = KMeans(n_clusters=k,max_iter=param['max_iter'],n_init=param['n_init'],init=param['init'],n_jobs=-1)
        kmeans.fit(X)
        meandistortions.append(sum(np.min(cdist(X, kmeans.cluster_centers_, 'euclidean'), axis=1)) / X.shape[0])
    return meandistortions

def plot(K, meandistortions, out_filepath):
    fig = plt.figure()
    plt.plot(K, meandistortions, 'bx-')

    plt.xlabel('k')
    plt.ylabel('Average distortion')
    plt.title('Selecting k with the Elbow Method')
    # plt.show()

    with PdfPages(out_filepath) as pdf:
        pdf.savefig(fig)

def main(argv):
    kmeans_model_dir = '/home/tor/sun4/xprmnt/segment-sorter/kmeans-model/msrc21/'
    kmeans_model_name = 'kmeans.20141210.000921'

    print('reading X ...')
    X_filepath = kmeans_model_dir + '/' + kmeans_model_name + '.inputdata'
    X = np.genfromtxt(X_filepath, delimiter=',')
    # X = X[:1000,:]
    print('n_sample= %i' % (X.shape[0]))
    print('n_feature= %i' % (X.shape[1]))

    param = {}
    param['max_iter'] = 300
    param['n_init'] = 10
    param['init'] = 'k-means++'

    kmin = 100
    kmax = 10000
    n_step = 50
    K = [int(i) for i in np.linspace(kmin, kmax, num=n_step)]
    print K

    meandistortions = elbow_method(K, X, param)
    plot(K,meandistortions,kmeans_model_dir+'/'+kmeans_model_name+'.elbowmethod.pdf')

if __name__ == '__main__':
    main(sys.argv)