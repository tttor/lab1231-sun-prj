import csv
from os import listdir
from os.path import isfile, join

from sklearn.grid_search import GridSearchCV
from sklearn.linear_model import SGDClassifier
from sklearn.svm import NuSVC
from sklearn.neighbors import KNeighborsClassifier #from sklearn.neighbors import RadiusNeighborsClassifier
from sklearn.ensemble import ExtraTreesClassifier
from sklearn.ensemble import GradientBoostingClassifier

from lab1231_sun_prj.util import evaluator

def load_oliv8():
    ''' This load the data D={(x,y)}, where x and y are the gist and the label, respectively.'''
    print 'load_oliv8(): BEGIN'
    
    str2num_label_dict = {'coast':1, 'forest':2, 'highway':3, 'insidecity':4, 'mountain':5, 'opencountry':6, 'street':7, 'tallbuilding':8}
    
    dir_path = '/home/tor/sun/ws/lab1231-sun-prj/cpp-ws/out/gist/'
    files = [ f for f in listdir(dir_path) if isfile(join(dir_path,f)) ]
    
    D = []
    for i, f in enumerate(files):
        #print "load ith=", i+1, " of ", len(files)
        X = None
        with open(dir_path + f, 'rb') as csvfile:
            content = csv.reader(csvfile)
            x = [row[0] for row in content]
            y = str2num_label_dict[ f[0:f.index('_')] ]
        D.append( (x,y) );
    
    #y = [datum[1] for datum in D]
    #print 'n_instance=', len(y)
    #uniq_y = set(y)
    #for i in uniq_y:
        #print 'label=', i, 'has', y.count(i)
    
    print 'load_oliv8(): END'    
    return D

def evaluate(clf_type, data):
    print 'evaluate(): BEGIN'
    
    X_tr = data[0]
    X_te = data[1]
    y_tr = data[2]
    y_te = data[3]
    
    meta_clf = tune(clf_type, X_tr, y_tr)
    clf_star = train(meta_clf, X_tr, y_tr)
    perf = test(clf_star, X_te, y_te)
    
    print 'evaluate(): END'
    return (clf_star, perf)
    
def tune(clf_type, X_tr, y_tr):
    ''' Return the best classifier of type of clf given a training data tr_data, its hyperparams are tuned using k-fold CV'''
    print 'tuning ...'
    
    meta_clf = None
    if clf_type == 'nuSVC':
        meta_clf = tune_nuSVC(X_tr, y_tr)        
    #elif clf_type == 'SGD':
        #meta_clf = tune_SGD(X_tr, y_tr)
    #elif clf_type == 'NearestNeigbours':
        #meta_clf = tune_NearestNeigbours(X_tr, y_tr)
    #elif clf_type == 'XRandomizedTrees':
        #meta_clf = tune_XRandomizedTrees(X_tr, y_tr)
    #elif clf_type == 'GradientBoosting':
        #meta_clf = tune_GradientBoosting(X_tr, y_tr)
    else:
        assert False, "UNKNOWN clf!" 
    
    return meta_clf
    
def train(clf, X_tr, y_tr):
    print 'training ...'
    clf.fit(X_tr, y_tr)
    return clf

def test(clf, X_te, y_te):
    print 'testing ...'
    y_pred = clf.predict(X_te)
    perf = evaluator.compute(y_te, y_pred)
    
    return perf

def tune_nuSVC(X_tr, y_tr):
    clf = NuSVC()
    
    #param_space = {
                    ##'nu': [0.3, 0.5, 0.8], \
                    #'kernel': ['rbf'], \ 
                    #'degree': [3, 5], \
                    ##'gamma': [0.0, 0.5],
                  #}
    
    #grid_search = GridSearchCV(clf, param_grid=param_space, cv=10)
    #grid_search.fit(X_tr,y_tr)# Run fit with all sets of parameters.
    
    #return grid_search.get_params()['estimator']
    return clf
    
#def tune_SGD(X_tr, y_tr):
    #print 'tune_SGD(X_tr, y_tr):'
    #clf = SGDClassifier()
    
    #param_space = {'loss': ['hinge', 'log', 'modified_huber'], \
                   #'penalty': ['l2', 'l1'], \
                   #'alpha' : [0.0001, 0.001]}
    
    #grid_search = GridSearchCV(clf, param_grid=param_space, cv=10)
    #grid_search.fit(X_tr,y_tr)# Run fit with all sets of parameters.
    
    #return grid_search.get_params()['estimator']
    
#def tune_NearestNeigbours(X_tr, y_tr):
    #print 'tune_NearestNeigbours(X_tr, y_tr):'
    #clf = KNeighborsClassifier(algorithm='ball_tree')
    
    #param_space = {'n_neighbors': [3, 5, 10], \
                   #'leaf_size': [20, 30, 50], \
                   #'metric': ['minkowski']}# 'euclidean' is unapplicable to the ball_tree algorithm, but for brute

    #grid_search = GridSearchCV(clf, param_grid=param_space, cv=10)
    #grid_search.fit(X_tr,y_tr)# Run fit with all sets of parameters.
    
    #return grid_search.get_params()['estimator']

#def tune_XRandomizedTrees(X_tr, y_tr):
    #print 'tune_XRandomizedTrees(X_tr, y_tr):'
    #clf = ExtraTreesClassifier()
    
    #param_space = {'n_estimators': [10, 30, 70], \
                   #'criterion': ['gini', 'entropy'], \
                   #'max_features': [0.3, 0.5]}
    
    #grid_search = GridSearchCV(clf, param_grid=param_space, cv=10)
    #grid_search.fit(X_tr,y_tr)# Run fit with all sets of parameters.
    
    #return grid_search.get_params()['estimator']

#def tune_GradientBoosting(X_tr, y_tr):
    #print 'tune_GradientBoosting(X_tr, y_tr):'
    #clf = GradientBoostingClassifier()
    
    #param_space = {'n_estimators': [50, 100, 200], \
                   #'learning_rate': [0.1, 0.3], \
                   #'max_features': [0.3, 0.5]}
    
    #grid_search = GridSearchCV(clf, param_grid=param_space, cv=10)
    #grid_search.fit(X_tr,y_tr)# Run fit with all sets of parameters.
    
    #return grid_search.get_params()['estimator']
