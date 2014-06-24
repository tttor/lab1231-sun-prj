#!/usr/bin/python

import numpy as np
from lab1231_sun_prj.clf import scene_clf
from lab1231_sun_prj.util import plotter
from sklearn.cross_validation import train_test_split

def main():
    #
    data = scene_clf.load_oliv8()
    X = [datum[0]for datum in data]
    y = [datum[1] for datum in data]
    
    n_clone = 1# a single clone is represented as a list of [X_tr, X_te, y_tr, y_te]
    dataset = [train_test_split(X, y, test_size=0.3, random_state=i) for i in range(n_clone)]

    # Preprocessing
    # TODO
    
    # Tune, train, test
    #clfs = ['SGD', 'nuSVC', 'NearestNeigbours', 'XRandomizedTrees', 'GradientBoosting']
    clfs = ['nuSVC']
    
    clf_perf_list = [scene_clf.evaluate(clf, data) for clf in clfs for data in dataset]
    clf_stars = [e[0] for e in clf_perf_list]
    perf_metrics = [e[1] for e in clf_perf_list]
    
    # Convert to 2D array of element e_{i,j} from classifier c_i and dataset D_j
    clf_stars = np.asarray(clf_stars)
    clf_stars = clf_stars.reshape(len(clfs), len(dataset))
    
    perf_metrics = np.asarray(perf_metrics)
    perf_metrics = perf_metrics.reshape(len(clfs), len(dataset))
    
    # Plot
    plotter.plot_1(perf_metrics, clfs)
    plotter.plot_2(perf_metrics, clfs)
    plotter.plot_cm(perf_metrics, clfs)
    
if __name__ == "__main__":
    main()
