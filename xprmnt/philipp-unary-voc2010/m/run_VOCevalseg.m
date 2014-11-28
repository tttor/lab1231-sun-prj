clear all
clc

% change this path if you install the VOC code elsewhere
VOCcode_dir = '../../../external/VOCdevkit/VOCcode';
addpath(VOCcode_dir);

%% initialize VOC options
% experiment id, becomes part of the prediction dirname
id = 'merged';

% The list_filename without any extension, e.g.: 'val'
VOCopts.testset = 'Test';

% The dirpath of target list: /home/tor/sun3/dataset/pascal/VOC2012/VOCdevkit/VOC2012/ImageSets/Segmentation/%s.txt
VOCopts.seg.imgsetpath = '/home/tor/sun4/xprmnt/philipp-unary-voc2010/meta/split_voc2010_philipp/%s.txt';

% The dirpath of .png predictions: /home/tor/sun3/dataset/pascal/VOC2012/VOCdevkit/results/VOC2012/Segmentation/%s_%s_cls/%s.png
VOCopts.seg.clsrespath = '/home/tor/sun4/xprmnt/philipp-unary-voc2010/result/%s_%s_cls/%s.png';

% The dirpath of .png GT: /home/tor/sun3/dataset/pascal/VOC2012/VOCdevkit/VOC2012/SegmentationClass/%s.png
VOCopts.seg.clsimgpath = '/home/tor/sun3/dataset/pascal/VOC2012/VOCdevkit/VOC2012/SegmentationClass/%s.png';

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