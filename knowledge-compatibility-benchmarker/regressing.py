#!/usr/bin/python

from sklearn.svm import NuSVR
import numpy as np

def main():
    #
    regression_data_filepath = '/home/tor/sun4/xprmnt/knowledge-compatibility-benchmarker-cvpr2015/regression-data/regression_data.csv'
    data = np.genfromtxt(regression_data_filepath, delimiter=',')

    #
    X = data[:,1:-1]
    y = data[:,-1]
    
    #
    clf = NuSVR(C=1.0, nu=0.1)
    clf.fit(X, y) 

if __name__ == '__main__':
    main()