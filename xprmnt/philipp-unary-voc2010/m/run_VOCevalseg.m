clear all
clc

% change this path if you install the VOC code elsewhere
VOCcode_dir = '../../../external/VOCdevkit/VOCcode';
addpath(VOCcode_dir);

%% initialize VOC options
% experiment id, becomes part of the prediction dirname
id = 'split_voc2010_philipp';

% The list_filename without any extension, e.g.: 'val'
VOCopts.testset = 'Test';

% The dirpath of target list: <...>/Segmentation/%s.txt
test_data_list_dir = '/home/tor/dataset/pascal/VOC2010/meta/split_voc2010_philipp';
VOCopts.seg.imgsetpath = strcat(test_data_list_dir, '/%s.txt');%<VOCopts.testset>.txt

% The dirpath of .png GT:<...>/SegmentationClass/%s.png
gt_annotation_dir = '/home/tor/dataset/pascal/VOC2010/VOCdevkit/VOC2010/SegmentationClass-scaled-0.25';
VOCopts.seg.clsimgpath = strcat(gt_annotation_dir,'/%s.png');

% The dirpath of .png predictions: <...>/Segmentation/%s_%s_cls/%s.png
predicted_annotation_dir = '/home/tor/xprmnt/generating-philipp-unary-voc2010/unary-scaled/annotation';
VOCopts.seg.clsrespath = strcat(predicted_annotation_dir,'/%s_%s_cls/%s.png');% <id>_<VOCopts.testset>_cls/<filename>.png

% This does not include the class of 'background'
VOCopts.nclasses = 20;
VOCopts.classes = {...
    'aeroplane'
    'bicycle'
    'bird'
    'boat'
    'bottle'
    'bus'
    'car'
    'cat'
    'chair'
    'cow'
    'diningtable'
    'dog'
    'horse'
    'motorbike'
    'person'
    'pottedplant'
    'sheep'
    'sofa'
    'train'
    'tvmonitor'};

[accuracies,avacc,conf,rawcounts] = VOCevalseg(VOCopts,id);