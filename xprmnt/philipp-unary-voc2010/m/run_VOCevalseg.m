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

% The dirpath of target list: /home/tor/sun3/dataset/pascal/VOC2012/VOCdevkit/VOC2012/ImageSets/Segmentation/%s.txt
test_data_list_dir = '/home/tor/sun4/xprmnt/philipp-unary-voc2010/meta/split_voc2010_philipp';
VOCopts.seg.imgsetpath = strcat(test_data_list_dir, '/%s.txt');%<VOCopts.testset>.txt

% The dirpath of .png predictions: /home/tor/sun3/dataset/pascal/VOC2012/VOCdevkit/results/VOC2012/Segmentation/%s_%s_cls/%s.png
predicted_annotation_dir = '/home/tor/sun4/xprmnt/own-unary-voc2010/annotation/with-own-unaries-20141218';
VOCopts.seg.clsrespath = strcat(predicted_annotation_dir,'/%s_%s_cls/%s.png');% <id>_<VOCopts.testset>_cls/<filename>.png

% The dirpath of .png GT: /home/tor/sun3/dataset/pascal/VOC2012/VOCdevkit/VOC2012/SegmentationClass/%s.png
gt_annotation_dir = '/home/tor/sun3/dataset/pascal/VOC2012/VOCdevkit/VOC2012/SegmentationClass';
VOCopts.seg.clsimgpath = strcat(gt_annotation_dir,'/%s.png');

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