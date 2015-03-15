import sys
import matplotlib.pyplot as plt
import numpy as np

from sklearn import preprocessing

def main(argv):
    assert len(argv)==3, 'INSUFFICENT NUMBER OF ARGUMENTS'

    regression_data_dirpath = argv[1]
    fea_name = argv[2]

    X_filepath = regression_data_dirpath+'/'+fea_name+'.csv'
    X = np.genfromtxt(X_filepath, delimiter=',')
    n_sample = X.shape[0]
    n_cooccurrence_fea = X.shape[1]

    # Standardization for standard normally distributed data: Gaussian with zero mean and unit variance.
    X_scaled = preprocessing.scale(X)
    # print X_scaled.mean(axis=0)
    # print X_scaled.std(axis=0)

    X_scaled_filepath = regression_data_dirpath+'/'+fea_name+'_scaled.csv'
    np.savetxt(X_scaled_filepath, X_scaled, delimiter=",")

    # Min Mix Scaling
    X_scaled_min_max = preprocessing.MinMaxScaler().fit_transform(X)

    X_scaled_min_max_filepath = regression_data_dirpath+'/'+fea_name+'_scaled_min_max.csv'
    np.savetxt(X_scaled_min_max_filepath, X_scaled_min_max, delimiter=",")

    # # Plotting
    # for i in range(n_cooccurrence_fea):
    #     plt.clf()        
    #     plt.scatter(range(1,n_sample+1), X_scaled[:,i], edgecolors='none')
    #     plt.grid(True)
    #     plt.savefig(regression_data_dirpath+'/input/cooccurrence_fea_'+str(i)+'_scatter.pdf')
    #     plt.savefig(regression_data_dirpath+'/input/cooccurrence_fea_'+str(i)+'_scatter.png')

    #     plt.clf()
    #     n_bin = 50
    #     plt.hist(X_scaled[:,i], n_bin, normed=1, facecolor='red', alpha=0.5)
    #     plt.savefig(regression_data_dirpath+'/input/cooccurrence_fea_'+str(i)+'_hist.pdf')
    #     plt.savefig(regression_data_dirpath+'/input/cooccurrence_fea_'+str(i)+'_hist.png')
    
if __name__ == '__main__':
    main(sys.argv)