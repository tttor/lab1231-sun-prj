clear all;
clc;

%%
gt_png_filepath = '/home/tor/sun4/xprmnt/philipp-unary-voc2010/debug/2007_000063.png';
gt_csv_filepath = '/home/tor/sun4/xprmnt/philipp-unary-voc2010/debug/2007_000063.csv';

util_dir = '/home/tor/sun/ws/lab1231-sun-prj/util/src-m';
addpath(util_dir);

VOCcode_dir = '/home/tor/sun3/dataset/pascal/VOC2012/VOCdevkit/VOCcode';
addpath(VOCcode_dir);

cmap = make_csv_from_png_voc(gt_png_filepath, gt_csv_filepath);
assert( isequal(cmap,VOClabelcolormap(256)), 'isequal(cmap,VOClabelcolormap(256)): FALSE' )

%%
newgt_png_filepath = '/home/tor/sun4/xprmnt/philipp-unary-voc2010/debug/2007_000063_new.png';
make_png_from_csv_voc(gt_csv_filepath, newgt_png_filepath, cmap)

%%
[gt, gt_map] = imread(gt_png_filepath);
[newgt, newgt_map] = imread(newgt_png_filepath);

assert( isequal(newgt, gt), 'isequal(newgt, gt): FALSE' )