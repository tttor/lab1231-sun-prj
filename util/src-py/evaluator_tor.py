#!/usr/bin/python

import sys
import numpy as np
from sklearn.metrics import confusion_matrix
from lxml import etree
from pascal_voc_2012 import translate
from pascal_voc_2012 import class_name_map

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
    '''
    @brief Obtain the performance of prediction on i-th class based on the given confusion matrix cm.
    '''
    n_tp = cm[i,i]
    n_fp = sum(cm[:,i])- n_tp
    n_fn = sum(cm[i,:]) - n_tp
    acc = get_acc_voc(n_tp, n_fp, n_fn)

    perf = {'n_tp': n_tp, 'n_fp': n_fp, 'n_fn': n_fn, 'acc': acc}

    return perf

def write_eval(class_avg_acc_data, global_acc_data, out_dir, filename):
    '''
    @brief: write the results of eval on a single prediction.
    \class_avg_acc_data a map that maps a class to the performance of predicting that class
    \out_dir
    \filename
    '''
    #
    global_acc = float(global_acc_data['total_n_tp'])/global_acc_data['total_n_pixel']
    class_avg_acc = get_class_avg_acc(class_avg_acc_data);

    #
    root = etree.Element("evaluation")
    root.set('target', filename[0:len(filename)-4])
    root.set('class_average_accuracy', str(class_avg_acc))
    root.set('global_accuracy', str(global_acc))
    root.set('total_n_tp', str(global_acc_data['total_n_tp']))
    root.set('total_n_pixel', str(global_acc_data['total_n_pixel']))

    for key, val in class_avg_acc_data.iteritems():
        sub = etree.SubElement(root, 'class')
        sub.set('num', str(key))
        sub.set('name', class_name_map[key])

        subsub = etree.SubElement(sub, 'metric')
        for subkey, subval in val.iteritems():
            subsub.set(subkey, str(subval))

    xml_filepath = out_dir+'/'+filename 
    writer = etree.ElementTree(root)
    writer.write(xml_filepath, pretty_print=True)

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

    #
    class_avg_acc_data = {}
    for i in range(len(present_classes)):
        class_avg_acc_data[i] = get_perf(cm, i)

    # 
    global_acc_data = {}
    global_acc_data['total_n_tp'] = sum([cm[i][i] for i in range(cm.shape[0])])
    global_acc_data['total_n_pixel'] = sum(cm.flatten())

    return (class_avg_acc_data,global_acc_data) 

def eval(target_list, pred_dir, gt_dir):
    '''
    @brief eval a list of (more than one) targets
    '''            
    data = [ {'pred': pred_dir+'/'+target+'.csv', 'true': gt_dir+'/'+target+'.csv'} for target in target_list ]

    class_avg_acc_data = {}
    global_acc_data = {'total_n_tp': 0, 'total_n_pixel': 0}
    for i, datum in enumerate(data):
        print "single_eval on", str(i+1),"of", str(len(data))
        single_class_perf_map, single_global_acc_data = single_eval(datum['pred'], datum['true'])
        write_eval(single_class_perf_map, single_global_acc_data, out_dir=pred_dir, filename=(target_list[i]+'.evl'))

        for key, val in single_class_perf_map.iteritems():
            if key in class_avg_acc_data:
                class_avg_acc_data[key] = add_perf(class_avg_acc_data[key], val)
            else:
                class_avg_acc_data[key] = val

        global_acc_data['total_n_tp'] += single_global_acc_data['total_n_tp']
        global_acc_data['total_n_pixel'] += single_global_acc_data['total_n_pixel']

    return (class_avg_acc_data,global_acc_data)

def get_class_avg_acc(class_avg_acc_data):
    '''
    @brief get_class_avg_acc
    '''
    accuracies = [ val['acc'] for val in class_avg_acc_data.itervalues() ]
    return sum(accuracies)/len(accuracies)

def get_class_avg_acc_from_file(filepath):
    tree = etree.parse(filepath)
    parent = tree.getroot()

    acc_list = []
    for child in parent:
        for grandchild in child:
            acc_list.append(float(grandchild.attrib['acc']))

    return sum(acc_list)/len(acc_list)

def get_global_acc(data):
    return float(data['total_n_tp'])/data['total_n_pixel']

def main():
    assert len(sys.argv)==4, 'INSUFFICIENT NUMBER OF ARGUMENTS'

    target_listpath = sys.argv[1]
    pred_dir = sys.argv[2]
    gt_dir = sys.argv[3]

    with open(target_listpath) as f:
        target_list = f.readlines()
    target_list = [x.strip('\n') for x in target_list]
    target_list = [x[0:-4] for x in target_list if x[-4:]=='.csv']

    # class_avg_acc_data contains the mapping for _all_ given targets
    (class_avg_acc_data,global_acc_data) = eval(target_list, pred_dir, gt_dir)

    class_avg_acc = get_class_avg_acc(class_avg_acc_data)
    print 'class_avg_acc=', class_avg_acc

    global_acc = get_global_acc(global_acc_data)
    print 'global_acc=', global_acc

if __name__ == '__main__':
    main()