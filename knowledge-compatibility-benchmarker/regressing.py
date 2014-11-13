#!/usr/bin/python

from sklearn.svm import NuSVR
from sklearn.cross_validation import train_test_split
from sklearn.cross_validation import train_test_split
from sklearn.grid_search import GridSearchCV
from matplotlib.backends.backend_pdf import PdfPages
from sklearn.metrics import mean_squared_error
from sklearn.metrics import r2_score
import matplotlib.pyplot as plt
import numpy as np

def tune_NuSVR(X_tr, y_tr):
    print 'tune_nuSVR(X_tr, y_tr):'
    regressor = NuSVR()
    
    param_space = {'nu': [0.3], \
                   'kernel': ['linear'], \
                   'degree': [5], \
                   'gamma': [0.1]}
    
    grid_search = GridSearchCV(regressor, param_grid=param_space, cv=10)# use r2_score
    grid_search.fit(X_tr,y_tr)# Run fit with all sets of parameters.
    
    return grid_search.get_params()['estimator']

def train(clf, X_tr, y_tr):
    clf.fit(X_tr, y_tr)
    return clf

def test(clf, X_te, y_te):
    y_pred = clf.predict(X_te)
    y_true = y_te

    perf = {}
    perf['mse'] = mean_squared_error(y_true, y_pred) 
    perf['r2_score'] = r2_score(y_true, y_pred)
    perf['y_pred'] = y_pred
    perf['y_true'] = y_true
     
    return perf

def main():
    #
    regression_data_dir = '/home/tor/sun4/xprmnt/knowledge-compatibility-benchmarker-cvpr2015/regression-data'
    regression_data_filename  = 'voc2010_philippunary.csv'
    regression_data_filepath = regression_data_dir + '/' + regression_data_filename
    data = np.genfromtxt(regression_data_filepath, delimiter=',')

    #
    X = data[:,0:2]# TODO include all but the last column
    y = data[:,-1] # target is at the last column

    plt.scatter(range(len(y)), y)
    plt.grid(True)
    plt.show()

    # Shuffle n_clone times
    # NOTE: a single dataset is a list of [X_tr, X_te, y_tr, y_te]
    n_clone = 2
    datasets = [train_test_split(X, y, test_size=0.3, random_state=i) for i in range(n_clone)]

    #
    perf_of_datasets = []
    for dataset in datasets:
        X_tr, X_te, y_tr, y_te = dataset

        meta_regressor = tune_NuSVR(X_tr, y_tr)
        star_regressor = train(meta_regressor, X_tr, y_tr)
        perf = test(star_regressor, X_te, y_te)

        perf_of_datasets.append(perf)

    #
    figs = []
    for perf in perf_of_datasets:
        fig, ax = plt.subplots()
        scatter_plot = ax.scatter(perf['y_true'], perf['y_pred'])

        ax.set_ylabel('y_pred')
        ax.set_xlabel('y_true')
        ax.set_title('Performance of NuSVR: MSE= '+ str(perf['mse']))

        figs.append(fig)

    out_dir = '/home/tor/sun4/xprmnt/knowledge-compatibility-benchmarker-cvpr2015/regression-eval'
    out_filepath = out_dir + '/result.pdf'

    with PdfPages(out_filepath) as pdf:
        for fig in figs:
            pdf.savefig(fig)

if __name__ == '__main__':
    main()