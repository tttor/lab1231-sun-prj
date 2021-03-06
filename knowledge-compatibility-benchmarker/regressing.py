#!/usr/bin/python

# from matplotlib.backends.backend_pdf import PdfPages
import matplotlib
matplotlib.use('Agg')# Force matplotlib to not use any Xwindows backend.
import matplotlib.pyplot as plt
import numpy as np
import sys
import cPickle
import json
import yaml

from sklearn.cross_validation import train_test_split
from sklearn.grid_search import GridSearchCV
from sklearn.metrics import mean_squared_error
from sklearn.metrics import r2_score
from sklearn import cross_validation
from sklearn import preprocessing

from sklearn.svm import NuSVR
from sklearn.linear_model import Lasso
from sklearn.tree import DecisionTreeRegressor
from sklearn.ensemble import AdaBoostRegressor
from sklearn.ensemble import GradientBoostingRegressor
from sklearn import gaussian_process

def tune_NuSVR(X_tr, y_tr, hyperparam_filepath, meta_filepath):
    with open(hyperparam_filepath) as f:  
        param_space = yaml.load(f)

    with open(meta_filepath,'w') as f:
        json.dump(param_space, f)  

    #
    regressor = NuSVR()
    return tune(regressor, param_space, X_tr, y_tr)

def tune_GradientBoostingRegressor(X_tr, y_tr, hyperparam_filepath, meta_filepath):
    with open(hyperparam_filepath) as f:  
        param_space = yaml.load(f)

    with open(meta_filepath,'w') as f:
        json.dump(param_space, f)  

    regressor = GradientBoostingRegressor()
    return tune(regressor, param_space, X_tr, y_tr)

def tune_Lasso(X_tr, y_tr):
    param_space = {'alpha': [0.1, 0.3, 0.5, 0.7, 1.0],
                   'max_iter': [1000, 10000, 100000]}

    regressor = Lasso()
    return tune(regressor, param_space, X_tr, y_tr)

def tune_DecisionTreeRegressionwithAdaBoost(X_tr, y_tr):
    param_space = {'n_estimators': [300, 500, 1000],
                   'learning_rate': [0.5, 1.0],
                   'loss': ['linear']}

    regressor = AdaBoostRegressor(DecisionTreeRegressor(max_depth=4))
    return tune(regressor, param_space, X_tr, y_tr)

def tune_GP(X_tr, y_tr):
    param_space = {}

    regressor = gaussian_process.GaussianProcess(theta0=0.1, thetaL=.001, thetaU=1.)
    return tune(regressor, param_space, X_tr, y_tr)

def tune(regressor, param_space, X_tr, y_tr):
    print 'tune estimator ...'
    kf_cv = cross_validation.KFold(n=len(y_tr), n_folds=10)

    # use r2_score by default for regression
    grid_search = GridSearchCV(regressor, param_grid=param_space, cv=kf_cv)
    
    # Run fit() with all sets of parameters in param_space.
    grid_search.fit(X_tr,y_tr)

    return grid_search.best_estimator_   

def train(estimator, X_tr, y_tr):
    print 'train estimator ...'
    estimator.fit(X_tr, y_tr)
    return estimator

def test(estimator, X_te, y_te):
    print 'test estimator ...'
    y_pred = estimator.predict(X_te)
    y_true = y_te

    n_pesimistic = sum([1 if y_true[i]>y_pred[i] else 0 for i in range(len(y_true))])
    n_optimistic = sum([1 if y_true[i]<y_pred[i] else 0 for i in range(len(y_true))])

    perf = {}
    perf['mse'] = mean_squared_error(y_true, y_pred) 
    perf['r2'] = r2_score(y_true, y_pred)
    perf['y_pred'] = y_pred
    perf['y_true'] = y_true
    perf['n_optimistic'] = n_optimistic
    perf['n_pesimistic'] = n_pesimistic
     
    return perf

def get_best_regressor(regressor_list, score_mode):
    score_list = [regressor['perf'][score_mode] for regressor in regressor_list]

    best_idx = None
    if score_mode=='mse':
        best_idx = score_list.index( min(score_list) )
    elif score_mode=='r2':
        best_idx = score_list.index( max(score_list) )
    else:
        assert False, 'UNKNOWN score_mode :('

    return regressor_list[best_idx]

def unique_rows(a):
    # http://stackoverflow.com/questions/8560440/...
    # ...removing-duplicate-columns-and-rows-from-a-numpy-2d-array

    a = np.ascontiguousarray(a)
    unique_a = np.unique(a.view([('', a.dtype)]*a.shape[1]))
    return unique_a.view(a.dtype).reshape((unique_a.shape[0], a.shape[1]))

def main(argv):
    assert len(argv)==7, 'INSUFFICENT NUMBER OF ARGUMENTS'
    data_dirpath = argv[1]
    result_dirpath = argv[2]
    meta_filepath = argv[3]
    method = argv[4]
    hyperparam_filepath = argv [5]
    n_dataset_clone = int(argv[6])

    # Load inputs
    X_cooccurrence_fea_filepath = data_dirpath+'/input/cooccurrence_fea.csv'
    X_cooccurrence_fea = np.genfromtxt(X_cooccurrence_fea_filepath, delimiter=',')

    X_sceneprop_fea_filepath = data_dirpath+'/input/sceneprop_fea.csv'
    X_sceneprop_fea = np.genfromtxt(X_sceneprop_fea_filepath, delimiter=',')

    X_relloc_fea_filepath = data_dirpath+'/input/relloc_fea.csv'
    X_relloc_fea = np.genfromtxt(X_relloc_fea_filepath, delimiter=',')

    X = np.concatenate((X_cooccurrence_fea, X_sceneprop_fea, X_relloc_fea), axis=1)

    # Load targets
    y_filepath = data_dirpath+'/output/ca.csv'
    y = np.genfromtxt(y_filepath, delimiter=',')# note: y.shape is one-element tuple
    y = y.reshape((len(y),1))

    # Load img id
    imgid_list_filepath = data_dirpath+'/ann_img_1928.list'
    imgid_list = None
    with open(imgid_list_filepath) as f:
        imgid_list = f.readlines()
    imgid_list = [i.strip('\n') for i in imgid_list]

    imgidx = np.asarray(range(len(imgid_list)))
    imgidx = imgidx.reshape( (len(imgid_list),1) )

    y_with_imgidx = np.concatenate((y, imgidx), axis=1)
    
    # Preprocess data: clean up
    # When using GaussianProcess, this aims to avoid
    # Exception: Multiple input features cannot have the same target value.
    # For other methods, this may improve the regression performance.
    # the idx of a unique row is based on the first occurrence of the row.
    unique_X = unique_rows(X)
    unique_X_idx = [X.tolist().index(i) for i in unique_X.tolist()]

    X = unique_X
    y_with_imgidx = y_with_imgidx[unique_X_idx]

    # Sanity checks
    for i in y.tolist():
        if np.isnan(i) or np.isinf(i):
            assert False, 'nan or inf values'
    for i in X.flatten().tolist():
        if np.isnan(i) or np.isinf(i):
            assert False, 'nan or inf values'

    assert X.shape[0]==y_with_imgidx.shape[0], 'X.shape[0]!=y_with_imgidx.shape[0]'
    n_sample = X.shape[0]
    n_fea = X.shape[1]
    print 'n_sample=', n_sample
    print 'n_fea=', n_fea

    # Shuffle n_dataset_clone times
    # NOTE: a single dataset is a list of [X_tr, X_te, y_tr, y_te]
    
    datasets = [train_test_split(X, y_with_imgidx, test_size=0.3, random_state=i) for i in range(n_dataset_clone)]

    # Tune, train and test on all datasets
    print 'method=', method

    regressor_list = []
    for i, dataset in enumerate(datasets):
        print '----------'
        print 'Running the pipeline on', i+1,'-th clone of',len(datasets)

        regressor_data = {}
        
        X_tr, X_te, y_tr_with_imgidx, y_te_with_imgidx = dataset
        y_tr = y_tr_with_imgidx[:,0]
        y_te = y_te_with_imgidx[:,0]
        regressor_data['imgid_tr'] = [imgid_list[int(i)] for i in y_tr_with_imgidx[:,1]]
        regressor_data['imgid_te'] = [imgid_list[int(i)] for i in y_te_with_imgidx[:,1]]
        
        # Preprocess
        # have tried scaling y, but result in larger mse
        # '_scaled_min_max' on X results in worse mse, compared to using 'StandardScaler()'
        print('preprocess...')
        scale_mode = 'StandardScaler()'
        scaler = preprocessing.StandardScaler().fit(X_tr)
        regressor_data['scaler'] = scaler

        X_tr = scaler.transform(X_tr) 
        X_te = scaler.transform(X_te)

        # Tune
        meta_regressor = None
        if method=='NuSVR':
            meta_regressor = tune_NuSVR(X_tr, y_tr, hyperparam_filepath, meta_filepath)
        elif method=="GradientBoostingRegressor":   
            meta_regressor = tune_GradientBoostingRegressor(X_tr, y_tr, hyperparam_filepath, meta_filepath)
        elif method=='Lasso':            
            meta_regressor = tune_Lasso(X_tr, y_tr)
        elif method=='DecisionTreeRegressionwithAdaBoost':
            meta_regressor = tune_DecisionTreeRegressionwithAdaBoost(X_tr, y_tr)
        elif method=="GP":
            meta_regressor = tune_GP(X_tr, y_tr)
        else:
            assert False, 'UNKNOWN regression methods'

        # Train
        regressor = train(meta_regressor, X_tr, y_tr)
        regressor_data['regressor'] = regressor

        # Test
        perf = test(regressor, X_te, y_te)
        regressor_data['perf'] = perf

        #
        regressor_list.append(regressor_data)

    # Log regression perf of all data clones
    mse_list = [i['perf']['mse'] for i in regressor_list]
    np.savetxt(result_dirpath+'/mse.all', np.asarray(mse_list), delimiter=',')

    r2_list = [i['perf']['r2'] for i in regressor_list]
    np.savetxt(result_dirpath+'/r2.all', np.asarray(r2_list), delimiter=',')

    n_pesimistic_list = [i['perf']['n_pesimistic'] for i in regressor_list]
    np.savetxt(result_dirpath+'/n_pesimistic.all', np.asarray(n_pesimistic_list), delimiter=',')

    n_optimistic_list = [i['perf']['n_optimistic'] for i in regressor_list]
    np.savetxt(result_dirpath+'/n_optimistic.all', np.asarray(n_optimistic_list), delimiter=',')

    # Log only the best regressor
    best_regressor = {}
    best_regressor['mse'] = get_best_regressor(regressor_list, 'mse')
    best_regressor['r2'] = get_best_regressor(regressor_list,'r2')

    for score_mode, regressor in best_regressor.iteritems():
        y_true = regressor['perf']['y_true']
        y_pred = regressor['perf']['y_pred']
        score = regressor['perf'][score_mode]

        # write delta(y_true, y_pred) on testing
        delta_dict = {}
        for i in range(len(y_true)):
            d = abs(y_pred[i]-y_true[i])
            imgid = regressor['imgid_te'][i]
            delta_dict[imgid] = (y_true[i],y_pred[i],d)

        with open(result_dirpath+'/best_regressor_wrt_'+score_mode+'.delta','w') as f:
            json.dump(delta_dict, f) 

        # plot y_pred vs y_true
        fig, ax = plt.subplots()
        scatter_plot = ax.scatter(y_true, y_pred)
        ax.plot([0.0, 1.0], [0.0, 1.0], '-', linewidth=2, color='red')# the line of y=x 

        ax.set_ylabel('$\hat{y}$: predicted averaged-CA',fontsize=20)
        ax.set_xlabel('$y$: true averaged-CA',fontsize=20)
        ax.set_title( 'Testing '+ method +': ' + score_mode + '= ' + str(round(score,3)),fontsize=15)
        
        xlim = (-0.2, 1.2)
        ylim = xlim
        plt.xlim(xlim)
        plt.ylim(ylim)
        ax.grid(True)

        plt.savefig(result_dirpath+'/best_ypred_vs_ytrue_wrt_'+score_mode+'.png',bbox_inches = 'tight')
        plt.close

        # with PdfPages(result_dirpath + '/best_ypred_vs_ytrue_wrt_'+score_mode+'.pdf') as pdf:
        #     pdf.savefig(fig)
        with open(result_dirpath+'/best_regressor_wrt_'+score_mode+'.param', 'w') as f:
            f.write(str(regressor['regressor']))
        with open(result_dirpath+'/best_regressor_wrt_'+score_mode+'.pickle', 'wb') as f:
            cPickle.dump(regressor['regressor'], f)
        with open(result_dirpath+'/best_regressor_wrt_'+score_mode+'.scaler.pickle', 'wb') as f:
            cPickle.dump(regressor['scaler'], f)
        with open(result_dirpath+'/best_regressor_wrt_'+score_mode+'.y_pred', 'w') as f:
            np.savetxt(f, np.asarray(y_pred), delimiter=",")
        with open(result_dirpath+'/best_regressor_wrt_'+score_mode+'.r2', 'w') as f:
            f.write(str(regressor['perf']['r2']))
        with open(result_dirpath+'/best_regressor_wrt_'+score_mode+'.mse', 'w') as f:
            f.write(str(regressor['perf']['mse']))

    #
    with open(meta_filepath,'a') as f:
        f.write('\n')
        f.write(scale_mode+'\n')
        f.write(method+'\n')
        f.write(str(n_sample)+'\n')
        f.write(str(n_fea)+'\n')

if __name__ == '__main__':
    main(sys.argv)
