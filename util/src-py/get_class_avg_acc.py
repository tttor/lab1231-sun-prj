#!/usr/bin/python

from sklearn.metrics import confusion_matrix
import numpy as np

from evaluator import Evaluator

def main():
    gt_csv_filepath = '/home/tor/sun3/dataset/pascal/VOC2012/VOCdevkit/VOC2012/SegmentationClass-csv/2011_003271.csv'

    gt = np.loadtxt(gt_csv_filepath, delimiter=',')

    y_true = gt.flatten()
    y_pred = y_true

    cm = confusion_matrix(y_true, y_pred)
    print cm
    
    # TODO @tttor: finish this class avarage acc
    # class_acc_list = Evaluator.calc_acc_each_class(cm)
    # Evaluator.get_avg_avv(class_acc_list)

if __name__ == "__main__":
    main()
