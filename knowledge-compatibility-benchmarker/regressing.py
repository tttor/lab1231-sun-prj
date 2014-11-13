#!/usr/bin/python

from sklearn.svm import NuSVR
from sklearn.cross_validation import train_test_split
from sklearn.grid_search import GridSearchCV
from matplotlib.backends.backend_pdf import PdfPages
from sklearn.metrics import mean_squared_error
from sklearn.metrics import r2_score
import matplotlib.pyplot as plt
import numpy as np

from sklearn import cross_validation
from sklearn.naive_bayes import GaussianNB
from sklearn.svm import SVC
from sklearn.datasets import load_digits
from sklearn.learning_curve import learning_curve

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

    # TODO: what kind of score is used here?
    train_sizes, train_scores, test_scores = learning_curve(
        estimator, X, y, cv=cv, n_jobs=n_jobs, train_sizes=train_sizes)
    
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

def tune_NuSVR(X_tr, y_tr, out_dir=None):
    print 'tune_nuSVR(X_tr, y_tr):'

    regressor = NuSVR()
    param_space = {'nu': [0.3], \
                   'kernel': ['linear'], \
                   'degree': [5], \
                   'gamma': [0.1]}
    kf_cv = cross_validation.KFold(n=len(y_tr), n_folds=10)

    #
    grid_search = GridSearchCV(regressor, param_grid=param_space, cv=kf_cv)# use r2_score by default for regression
    grid_search.fit(X_tr,y_tr)# Run fit with all sets of parameters.

    #  
    print 'plot_learning_curve()'
    title = 'Learning curve'
    ylim = (0.0, 0.50)
    fig = plot_learning_curve(regressor, title, X_tr, y_tr, ylim, cv=kf_cv, n_jobs=4)
    if out_dir!=None:
        with PdfPages(out_dir+'/learning_curve.pdf') as pdf:
            pdf.savefig(fig)

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
    out_dir = '/home/tor/sun4/xprmnt/knowledge-compatibility-benchmarker-cvpr2015/regression-eval'

    regression_data_filename  = 'voc2010_philippunary.csv'
    regression_data_filepath = regression_data_dir + '/' + regression_data_filename
    data = np.genfromtxt(regression_data_filepath, delimiter=',')

    #
    X = data[:,0:2]# TODO include all but the last column
    y = data[:,-1] # target is at the last column

    # plt.scatter(range(len(y)), y)
    # plt.grid(True)
    # plt.show()

    # Shuffle n_clone times
    # NOTE: a single dataset is a list of [X_tr, X_te, y_tr, y_te]
    n_clone = 2
    datasets = [train_test_split(X, y, test_size=0.3, random_state=i) for i in range(n_clone)]

    #
    perf_of_datasets = []
    for dataset in datasets:
        X_tr, X_te, y_tr, y_te = dataset

        meta_regressor = tune_NuSVR(X_tr, y_tr, out_dir)
        star_regressor = train(meta_regressor, X_tr, y_tr)
        perf = test(star_regressor, X_te, y_te)

        perf_of_datasets.append(perf)

    #
    figs = []
    for perf in perf_of_datasets:
        fig, ax = plt.subplots()
        scatter_plot = ax.scatter(perf['y_true'], perf['y_pred'])

        ax.set_ylabel('$y_{pred}$')
        ax.set_xlabel('$y_{true}$')
        ax.set_title( 'NuSVR: MSE= '+str(perf['mse'])+', r2_score= '+str(perf['r2_score']) )

        figs.append(fig)

    with PdfPages(out_dir + '/test_perf_ypred_vs_ytrue.pdf') as pdf:
        for fig in figs:
            pdf.savefig(fig)

if __name__ == '__main__':
    main()