#!/usr/bin/python
# This aims to collect the regression dataset $D={x_i,y_i}_{i=1}^{i=n}$.

import sys
import numpy as np
import matplotlib.pyplot as plt

import feature_extractor as xtt
import cooccurrence_knowledge as kc
import scene_property_knowledge as ks
import relative_location_knowledge as kr

def get_input(sample_id_list, ann_dir, knowledge_dir, regression_data_dir, ori_img_dir):
    '''
    Extract features from the annotation and knowledge
    '''
    ann_filepaths = [ann_dir+'/'+target+'.csv' for target in sample_id_list]
    n_sample = len(sample_id_list)

    cooccurrence_knowledge = kc.read(knowledge_dir+'/cooccurrence-knowledge/cooccurrence.xml')
    cooccurrence_fea = []

    sceneprop_knowledge = ks.read(knowledge_dir+'/scene-property-knowledge/scene_property.xml')
    sceneprop_fea = []

    relloc_knowledge = kr.read(knowledge_dir+'/relative-location-knowledge/relloc-pickle')
    relloc_fea = []

    for i, ann_filepath in enumerate(ann_filepaths):
        print 'Extracting feature:', str(i+1), 'of', str(len(ann_filepaths)), 'id=',sample_id_list[i]
        ann = {}
        ann['ann'] = np.loadtxt(ann_filepath, delimiter=',')
        ann['filename'] = sample_id_list[i]
        ann['ori_img_dir'] = ori_img_dir
        ann['ori_img_ext'] = '.jpg'

        #
        ith_cooccurrence_fea = xtt.extract_cooccurrence_fea(ann,cooccurrence_knowledge)
        cooccurrence_fea.append(ith_cooccurrence_fea)

        ith_sceneprop_fea = xtt.extract_sceneprop_fea(ann,sceneprop_knowledge)
        sceneprop_fea.append(ith_sceneprop_fea)

        ith_relloc_fea = xtt.extract_relloc_fea(ann,relloc_knowledge)
        relloc_fea.append(ith_relloc_fea)

    #
    cooccurrence_fea_filepath = regression_data_dir+'/'+'input.cooccurrence_fea.csv'
    np.savetxt(cooccurrence_fea_filepath, cooccurrence_fea, delimiter=",")

    sceneprop_fea_filepath = regression_data_dir+'/'+'input.sceneprop_fea.csv'
    np.savetxt(sceneprop_fea_filepath, sceneprop_fea, delimiter=",")

    relloc_fea_filepath = regression_data_dir+'/'+'input.relloc_fea.csv'
    np.savetxt(relloc_fea_filepath, relloc_fea, delimiter=",")

def get_output(sample_id_list, eval_dir, regression_data_dir):
    '''
    Extract regression outputs: averaged class accuracy (CA) and global accuracy (GA)
    '''
    print('Extract regression outputs ...')
    
    n_output_type = 2
    regression_output = np.zeros( (len(sample_id_list),n_output_type) )
    eval_dirpaths = [eval_dir+'/'+target+'-eval' for target in sample_id_list]

    for i,eval_dirpath in enumerate(eval_dirpaths):
        ca_filepath = eval_dirpath+'/'+sample_id_list[i]+'.avg_class_acc'
        ca = np.genfromtxt(ca_filepath, delimiter=',')
        ca = float(ca)/100.0# percent normalization

        ga_filepath = eval_dirpath+'/'+sample_id_list[i]+'.global_acc'
        ga = np.genfromtxt(ga_filepath, delimiter=',')
        ga = float(ga)/100.0# percent normalization

        regression_output[i,0] = ca
        regression_output[i,1] = ga

    # #
    # x_values = range(1,regression_output.shape[0]+1)
    # plt.scatter(x_values, regression_output[:,0], 
    #             c='red',edgecolors='none',alpha=0.3, label='CA')
    # plt.scatter(x_values, regression_output[:,1], 
    #             c='green',edgecolors='none',alpha=0.3, label='GA')
    
    # plt.legend()
    # plt.grid(True)

    # ca_scatter_plot_filepath = regression_data_dir+'/'+'ca_scatter_plot'
    # plt.savefig(ca_scatter_plot_filepath+'.pdf')
    # plt.savefig(ca_scatter_plot_filepath+'.png')
    # plt.close()

    #
    n_bin = 50

    plt.hist(regression_output[:,0], n_bin, normed=1, facecolor='red', alpha=0.5)
    ca_hist_plot_filepath = regression_data_dir+'/'+'ca_hist_plot'
    plt.savefig(ca_hist_plot_filepath+'.pdf')
    plt.savefig(ca_hist_plot_filepath+'.png')
    plt.close()

    
    plt.hist(regression_output[:,1], n_bin, normed=1, facecolor='green', alpha=0.5)
    ga_hist_plot_filepath = regression_data_dir+'/'+'ga_hist_plot'
    plt.savefig(ga_hist_plot_filepath+'.pdf')
    plt.savefig(ga_hist_plot_filepath+'.png')
    plt.close()

    #
    regression_output_filepath = regression_data_dir+'/'+'output.ca.csv'
    np.savetxt(regression_output_filepath, regression_output[:,0], delimiter=",")

    regression_output_filepath = regression_data_dir+'/'+'output.ga.csv'
    np.savetxt(regression_output_filepath, regression_output[:,1], delimiter=",")

def main(argv):
    assert len(argv)==7, 'INSUFFICENT NUMBER OF ARGUMENTS'
    ann_list_filepath = argv[1]
    knowledge_dir = argv[2]
    ann_dir = argv[3]
    eval_dir = argv[4]
    regression_data_dir = argv[5]
    ori_img_dir = argv[6]

    with open(ann_list_filepath) as f:
        sample_id_list = f.readlines()
    sample_id_list = [x.strip('\n') for x in sample_id_list]

    #
    get_input(sample_id_list, ann_dir, knowledge_dir, regression_data_dir, ori_img_dir)
    get_output(sample_id_list, eval_dir, regression_data_dir)
    
if __name__ == '__main__':
    main(sys.argv)

