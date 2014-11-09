#!/usr/bin/python

import numpy as np
from sklearn.metrics import confusion_matrix

def get_acc_voc(n_tp, n_fp, n_fn):
    return float(n_tp)/(n_tp+n_fp+n_fn)

def add_perf(perf_1, perf_2):
    perf_t = {}
    perf_t['n_tp'] = perf_1['n_tp'] + perf_2['n_tp']
    perf_t['n_fp'] = perf_1['n_fp'] + perf_2['n_fp']
    perf_t['n_fn'] = perf_1['n_fn'] + perf_2['n_fn']
    perf_t['acc'] = get_acc_voc(perf_t['n_tp'], perf_t['n_fp'], perf_t['n_fn'])

    return perf_t

def get_perf(cm, i):
    n_tp = cm[i,i]
    n_fp = sum(cm[:,i])- n_tp
    n_fn = sum(cm[i,:]) - n_tp
    acc = get_acc_voc(n_tp, n_fp, n_fn)

    perf = {'n_tp': n_tp, 'n_fp': n_fp, 'n_fn': n_fn, 'acc': acc}

    return perf

def write_single_eval():
    '''
    TODO @tttor
    @brief: write the results of eval on a single prediction.
    '''

def single_eval(pred_filepath, true_filepath):
    '''
    @brief: do evaluation on a single prediction
    '''
    # read and flatten
    y_pred_raw = np.loadtxt(open(pred_filepath,"rb"),delimiter=",").flatten()
    y_true_raw = np.loadtxt(open(true_filepath,"rb"),delimiter=",").flatten()
    assert len(y_pred_raw)==len(y_true_raw), 'len(y_pred_raw)!=len(y_true_raw)'

    # remove void
    void_num = 255
    nonvoid_idxes = [i for i in range(len(y_true_raw)) if y_true_raw[i]!=void_num]

    y_pred = [y_pred_raw[i] for i in nonvoid_idxes]
    y_true = [y_true_raw[i] for i in nonvoid_idxes]
    assert len(y_pred)==len(y_true), 'len(y_pred)!=len(y_true)'

    # get confusion mat
    cm = confusion_matrix(y_true, y_pred)

    # get present classes in an ascending order!, map to their accuracy
    present_classes = sorted( list(set(y_pred + y_true)) )

    class_perf_map = {}
    for i in range(len(present_classes)):
        class_perf_map[i] = get_perf(cm, i)

    return class_perf_map 

def eval(data):             
    class_perf_map = {}

    for i, datum in enumerate(data):
        print "single_eval " + str(i+1) +"/" + str(len(data))
        single_class_perf_map = single_eval(datum['pred'], datum['true'])

        for key, val in single_class_perf_map.iteritems():
            if key in class_perf_map:
                class_perf_map[key] = add_perf(class_perf_map[key], val)
            else:
                class_perf_map[key] = val

    return class_perf_map

def get_average_acc_over_classes(class_perf_map):
    accuracies = [ val['acc'] for val in class_perf_map.itervalues() ]
    return sum(accuracies)/len(accuracies)

def main():
    pred_dir = '/media/tor/46DAF35ADAF344A9/tor/robotics/prj/011/xprmnt/philipp-unary-voc2010/result/test-all'
    gt_dir = '/home/tor/sun3/dataset/pascal/VOC2012/VOCdevkit/VOC2012/SegmentationClass-csv'
    test_img_listpath = '/home/tor/sun4/xprmnt/philipp-unary-voc2010/meta/split_voc2010_philipp/Test.txt'

    with open(test_img_listpath) as f:
        test_img_list = f.readlines()
    test_img_list = [x.strip('\n') for x in test_img_list]
    # print test_img_list

    D_te = [ {'pred': pred_dir+'/'+test_img+'.csv', 'true': gt_dir+'/'+test_img+'.csv'} for test_img in test_img_list ]
    # print data

    class_perf_map = eval(D_te)
    # print class_perf_map

    average_acc_over_classes = get_average_acc_over_classes(class_perf_map)
    print average_acc_over_classes

if __name__ == '__main__':
    main()