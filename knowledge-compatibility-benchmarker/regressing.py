#!/usr/bin/python

import sys
from sklearn.svm import NuSVR
from sklearn.cross_validation import train_test_split
from sklearn.grid_search import GridSearchCV
from sklearn.metrics import mean_squared_error
from sklearn.metrics import r2_score
from sklearn import cross_validation
from sklearn.learning_curve import learning_curve
from matplotlib.backends.backend_pdf import PdfPages
import matplotlib.pyplot as plt
import numpy as np

def plot_learning_curve(estimator, title, X, y, ylim=None, cv=None,
                        n_jobs=1, train_sizes=np.linspace(.1, 1.0, 5)):
    """
    Generate a simple plot of the test and traning learning curve.

    Parameters
    ----------
    estimator : object type that implements the "fit" and "predict" methods
        An object of that type which is cloned for each validation.

    title : string
        Title for the chart.

    X : array-like, shape (n_samples, n_features)
        Training vector, where n_samples is the number of samples and
        n_features is the number of features.

    y : array-like, shape (n_samples) or (n_samples, n_features), optional
        Target relative to X for classification or regression;
        None for unsupervised learning.

    ylim : tuple, shape (ymin, ymax), optional
        Defines minimum and maximum yvalues plotted.

    cv : integer, cross-validation generator, optional
        If an integer is passed, it is the number of folds (defaults to 3).
        Specific cross-validation objects can be passed, see
        sklearn.cross_validation module for the list of possible objects

    n_jobs : integer, optional
        Number of jobs to run in parallel (default 1).
    """
    fig = plt.figure()
    plt.title(title)
    if ylim is not None:
        plt.ylim(*ylim)
    plt.xlabel("Training examples")
    plt.ylabel("Score")

    # scoring: Valid options are ['accuracy', 'adjusted_rand_score', 'average_precision', 'f1', 'log_loss', 'mean_absolute_error', 'mean_squared_error', 'precision', 'r2', 'recall', 'roc_auc']
    train_sizes, train_scores, test_scores = learning_curve(
        estimator, X, y, scoring='r2',cv=cv, n_jobs=n_jobs, train_sizes=train_sizes)
    
    train_scores_mean = np.mean(train_scores, axis=1)
    train_scores_std = np.std(train_scores, axis=1)
    test_scores_mean = np.mean(test_scores, axis=1)
    test_scores_std = np.std(test_scores, axis=1)
    plt.grid()

    plt.fill_between(train_sizes, train_scores_mean - train_scores_std,
                     train_scores_mean + train_scores_std, alpha=0.1,
                     color="r")
    plt.fill_between(train_sizes, test_scores_mean - test_scores_std,
                     test_scores_mean + test_scores_std, alpha=0.1, color="g")
    plt.plot(train_sizes, train_scores_mean, 'o-', color="r",
             label="Training score")
    plt.plot(train_sizes, test_scores_mean, 'o-', color="g",
             label="Cross-validation score")

    plt.legend(loc="best")

    return fig

def tune_NuSVR(X_tr, y_tr):
    print 'tune_nuSVR(X_tr, y_tr)...'

    param_space = {'C': [0.1, 0.3, 0.5, 0.7, 1.0],
                   'nu': [0.1, 0.3, 0.5, 0.7, 1.0], 
                   'kernel': ['linear', 'rbf', 'poly'], 
                   'degree': [3, 5, 7], 
                   'gamma': [0.0, 0.1, 0.3, 0.5, 0.7]}

    # dummy
    param_space = {'C': [1.0],
                   'nu': [0.5], 
                   'kernel': ['rbf'], 
                   'degree': [3], 
                   'gamma': [0.7]}

    #
    regressor = NuSVR()
    kf_cv = cross_validation.KFold(n=len(y_tr), n_folds=10)
    grid_search = GridSearchCV(regressor, param_grid=param_space, cv=kf_cv)# use r2_score by default for regression
    grid_search.fit(X_tr,y_tr)# Run fit with all sets of parameters.

    return grid_search.best_estimator_

def train(estimator, X_tr, y_tr):
    print 'train estimator ...'
    estimator.fit(X_tr, y_tr)
    return estimator

def test(estimator, X_te, y_te):
    print 'test estimator ...'
    y_pred = estimator.predict(X_te)
    y_true = y_te

    perf = {}
    perf['mse'] = mean_squared_error(y_true, y_pred) 
    perf['r2'] = r2_score(y_true, y_pred)
    perf['y_pred'] = y_pred
    perf['y_true'] = y_true
    perf['estimator'] = estimator
     
    return perf

def get_best_perf(perfs, scoring):
    score_list = [perf[scoring] for perf in perfs]

    best_idx = None
    if scoring=='mse':
        best_idx = score_list.index( min(score_list) )
    elif scoring=='r2':
        best_idx = score_list.index( max(score_list) )
    else:
        assert False, 'UNKNOWN scoring :('

    return perfs[best_idx]

def main(argv):
    assert len(argv)==3, 'INSUFFICENT NUMBER OF ARGUMENTS'
    regression_data_dir = argv[1]
    regression_output_dir = argv[2]

    #
    scaled = '_scaled' #: _scaled', '_scaled_min_max'

    X_cooccurrence_fea_filepath = regression_data_dir+'/input/cooccurrence_fea'+scaled+'.csv'
    X_cooccurrence_fea = np.genfromtxt(X_cooccurrence_fea_filepath, delimiter=',')

    X_sceneprop_fea_filepath = regression_data_dir+'/input/sceneprop_fea'+scaled+'.csv'
    X_sceneprop_fea = np.genfromtxt(X_sceneprop_fea_filepath, delimiter=',')

    X_relloc_fea_filepath = regression_data_dir+'/input/relloc_fea'+scaled+'.csv'
    X_relloc_fea = np.genfromtxt(X_relloc_fea_filepath, delimiter=',')

    X = np.concatenate((X_cooccurrence_fea, X_sceneprop_fea, X_relloc_fea), axis=1)

    y_filepath = regression_data_dir+'/output/ca.csv'
    y = np.genfromtxt(y_filepath, delimiter=',')

    assert X.shape[0]==y.shape[0], 'X.shape[0]!=y.shape[0]'
    n_sample = X.shape[0]
    print 'n_sample=', n_sample
    print 'n_input_fea=', X.shape[1]

    # Shuffle n_clone times
    # NOTE: a single dataset is a list of [X_tr, X_te, y_tr, y_te]
    n_clone = 5
    datasets = [train_test_split(X, y, test_size=0.3, random_state=i) for i in range(n_clone)]

    # Tune, train and test
    perf_of_datasets = []
    regressors = []
    for dataset in datasets:
        X_tr, X_te, y_tr, y_te = dataset

        meta_regressor = tune_NuSVR(X_tr, y_tr)
        regressor = train(meta_regressor, X_tr, y_tr)
        regressors.append(regressor)

        # #  
        # print 'plot_learning_curve()'
        # title = 'Learning curve'
        # ylim = (0.0, 1.0)
        # fig = plot_learning_curve(regressor, title, X_tr, y_tr, ylim, cv=10, n_jobs=4)
        # if regression_output_dir!=None:
        #     with PdfPages(regression_output_dir+'/learning_curve.pdf') as pdf:
        #         pdf.savefig(fig)

        #
        perf = test(regressor, X_te, y_te)
        perf_of_datasets.append(perf)

    #
    best_perf = {}
    best_perf['mse'] = get_best_perf(perf_of_datasets, 'mse')
    best_perf['r2'] = get_best_perf(perf_of_datasets, 'r2')

    #
    for scoring, perf in best_perf.iteritems():
        #
        fig, ax = plt.subplots()
        scatter_plot = ax.scatter(perf['y_true'], perf['y_pred'])
        ax.plot([0.0, 1.0], [0.0, 1.0], '-', linewidth=2, color='red')

        ax.set_ylabel('$y_{pred}$')
        ax.set_xlabel('$y_{true}$')
        ax.set_title( 'Testing NuSVR: ' + scoring + ' =' + str(perf[scoring]))
        
        xlim = (-0.2, 1.2)
        ylim = xlim
        plt.xlim(xlim)
        plt.ylim(ylim)
        ax.grid(True)

        plt.savefig(regression_output_dir + '/best_ypred_vs_ytrue_wrt_'+scoring+'.png')
        plt.close

        with PdfPages(regression_output_dir + '/best_ypred_vs_ytrue_wrt_'+scoring+'.pdf') as pdf:
            pdf.savefig(fig)

        #
        with open(regression_output_dir+'/best_regressor_wrt_'+scoring+'.param', 'w') as f:
            f.write(str(perf['estimator']))

if __name__ == '__main__':
    main(sys.argv)
