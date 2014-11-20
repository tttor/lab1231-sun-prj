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
    n_tp = cm[i,i]
    n_fp = sum(cm[:,i])- n_tp
    n_fn = sum(cm[i,:]) - n_tp
    acc = get_acc_voc(n_tp, n_fp, n_fn)

    perf = {'n_tp': n_tp, 'n_fp': n_fp, 'n_fn': n_fn, 'acc': acc}

    return perf

def write_eval(class_perf_map, out_dir, filename):
    '''
    @brief: write the results of eval on a single prediction.
    '''
    root = etree.Element("evaluation")
    root.set('target', filename[0:len(filename)-4])

    for key, val in class_perf_map.iteritems():
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

    class_perf_map = {}
    for i in range(len(present_classes)):
        class_perf_map[i] = get_perf(cm, i)

    return class_perf_map 

def eval(target_list, pred_dir, gt_dir):             
    data = [ {'pred': pred_dir+'/'+target+'.csv', 'true': gt_dir+'/'+target+'.csv'} for target in target_list ]

    class_perf_map = {}
    for i, datum in enumerate(data):
        print "single_eval on", str(i+1),"of", str(len(data))
        single_class_perf_map = single_eval(datum['pred'], datum['true'])
        write_eval(single_class_perf_map, out_dir=pred_dir, filename=(target_list[i]+'.evl'))

        for key, val in single_class_perf_map.iteritems():
            if key in class_perf_map:
                class_perf_map[key] = add_perf(class_perf_map[key], val)
            else:
                class_perf_map[key] = val

    return class_perf_map

def get_average_acc_over_classes(class_perf_map):
    accuracies = [ val['acc'] for val in class_perf_map.itervalues() ]
    return sum(accuracies)/len(accuracies)

def get_average_acc_over_clasess_from_file(filepath):
    tree = etree.parse(filepath)
    parent = tree.getroot()

    acc_list = []
    for child in parent:
        for grandchild in child:
            acc_list.append(float(grandchild.attrib['acc']))

    return sum(acc_list)/len(acc_list)

def main():
    assert len(sys.argv)==4, 'INSUFFICIENT NUMBER OF ARGUMENTS'

    target_listpath = sys.argv[1]
    pred_dir = sys.argv[2]
    gt_dir = sys.argv[3]

    with open(target_listpath) as f:
        target_list = f.readlines()
    target_list = [x.strip('\n') for x in target_list]
    target_list = [x[0:-4] for x in target_list if x[-4:]=='.csv']

    class_perf_map = eval(target_list, pred_dir, gt_dir)

    average_acc_over_classes = get_average_acc_over_classes(class_perf_map)
    print average_acc_over_classes

if __name__ == '__main__':
    main()