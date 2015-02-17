clear all;
clc;

util_dir = '/home/tor/lab1231-sun-prj/util/src-m';
addpath(util_dir);

VOCcode_dir = '/home/tor/dataset/pascal/VOC2010/VOCdevkit/VOCcode';
addpath(VOCcode_dir);

%%
cmap_256 = VOClabelcolormap(256);
csvwrite('cmap_256.csv', cmap_256);

cmap_255 = VOClabelcolormap(255);
csvwrite('cmap_255.csv', cmap_255);