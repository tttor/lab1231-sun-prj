% Create an image that represent the color of a class of VOC dataset
clear all;
clc;

result_dir = '/home/tor/sun3/dataset/pascal/VOC2010/VOCdevkit/VOC2010/class-color';

classes = 0:20;
classes = [classes, 255];% 255 -> void
n_class = length(classes);

N = 256;
cmap = VOClabelcolormap(N);

n_row = 300;
n_col = n_row;

for i=1:n_class
	c = classes(i);
	C = zeros(n_row,n_col) + c + 1;% plus one because imwrite will do minus one

	img_filepath = strcat(result_dir, '/class_', int2str(c), '.png');
	imwrite(C,cmap,img_filepath);
end