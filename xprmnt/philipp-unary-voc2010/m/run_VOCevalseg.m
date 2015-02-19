function [class_acc, global_acc] = run_VOCevalseg(ann_id_list_dirpath, ann_id_list_filename)

% change this path if you install the VOC code elsewhere
VOCcode_dir = '../../../external/VOCdevkit/VOCcode';
addpath(VOCcode_dir);

%%
xprmnt_id = 'split_voc2010_philipp';

png_gt_annotation_dir = '/home/tor/dataset/pascal/VOC2010/VOCdevkit/VOC2010/SegmentationClass';
dirpath_of_parent_dir_of_dir_containing_annotations = '/home/tor/xprmnt/philipp-unary-mrf-voc2010/result';

%%
VOCopts.testset = ann_id_list_filename;
VOCopts.seg.imgsetpath = strcat(ann_id_list_dirpath, '/%s.txt');%<VOCopts.testset>.txt
VOCopts.seg.clsimgpath = strcat(png_gt_annotation_dir,'/%s.png');
VOCopts.seg.clsrespath = strcat(dirpath_of_parent_dir_of_dir_containing_annotations,'/%s_%s_cls/%s.png');% <id>_<VOCopts.testset>_cls/<filename>.png
VOCopts.nclasses = 20;% This does not include the class of 'background'
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

%%
[class_acc, raw_avg_class_acc, conf_mat, raw_conf_mat, global_acc] = VOCevalseg(VOCopts,xprmnt_id);

end