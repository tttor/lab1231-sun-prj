#!/usr/bin/python
# This aims to collect the regression dataset $D={x_i,y_i}_{i=1}^{i=n}$.

import sys
sys.path.append("../../knowledge-compatibility-benchmarker")
sys.path.append("../../util/src-py")

import numpy as np
import matplotlib.pyplot as plt

from feature_extractor import extract_fea


def main(argv):
    assert len(argv)==6, 'INSUFFICENT NUMBER OF ARGUMENTS'

    # Extract features
    ann_list_filepath = argv[1]
    knowledge_dir = argv[2]
    ann_dir = argv[3]
    eval_dir = argv[4]
    regression_data_dirpath = argv[5]

    with open(ann_list_filepath) as f:
        target_list = f.readlines()
    target_list = [x.strip('\n') for x in target_list]
    ann_filepaths = [ann_dir+'/'+target+'.csv' for target in target_list]

    # fea_list = extract_fea(ann_filepaths, knowledge_dir)

    # Extract regression outputs: averaged class accuracy (CA) and global accuracy (GA)
    print('Extract regression outputs ...')
    
    n_output_type = 2
    regression_output = np.zeros( (len(target_list),n_output_type) )
    eval_dirpaths = [eval_dir+'/'+target+'-eval' for target in target_list]

    for i,eval_dirpath in enumerate(eval_dirpaths):
        ca_filepath = eval_dirpath+'/'+target_list[i]+'.avg_class_acc'
        ca = np.genfromtxt(ca_filepath, delimiter=',')

        ga_filepath = eval_dirpath+'/'+target_list[i]+'.global_acc'
        ga = np.genfromtxt(ga_filepath, delimiter=',')

        regression_output[i,0] = ca
        regression_output[i,1] = ga

    #
    x_values = range(1,regression_output.shape[0]+1)
    plt.scatter(x_values, regression_output[:,0], 
                c='red',edgecolors='none',alpha=0.3, label='CA')
    plt.scatter(x_values, regression_output[:,1], 
                c='green',edgecolors='none',alpha=0.3, label='GA')
    
    plt.legend()
    plt.grid(True)

    ca_scatter_plot_filepath = regression_data_dirpath+'/'+'ca_scatter_plot'
    plt.savefig(ca_scatter_plot_filepath+'.pdf')
    plt.savefig(ca_scatter_plot_filepath+'.png')
    plt.close()

    #
    n_bin = 50

    plt.hist(regression_output[:,0], n_bin, normed=1, facecolor='red', alpha=0.5)
    ca_hist_plot_filepath = regression_data_dirpath+'/'+'ca_hist_plot'
    plt.savefig(ca_hist_plot_filepath+'.pdf')
    plt.savefig(ca_hist_plot_filepath+'.png')
    plt.close()

    
    plt.hist(regression_output[:,1], n_bin, normed=1, facecolor='green', alpha=0.5)
    ga_hist_plot_filepath = regression_data_dirpath+'/'+'ga_hist_plot'
    plt.savefig(ga_hist_plot_filepath+'.pdf')
    plt.savefig(ga_hist_plot_filepath+'.png')
    plt.close()

    #
    regression_output_filepath = regression_data_dirpath+'/'+'output.ca.ga.csv'
    np.savetxt(regression_output_filepath, regression_output, delimiter=",")
    
if __name__ == '__main__':
    main(sys.argv)

