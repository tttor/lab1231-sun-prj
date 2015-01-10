clear all;
clc;

set_rescaling_param;
ori_jpg_dir = '/home/tor/sun3/dataset/pascal/VOC2010/VOCdevkit/VOC2010/JPEGImages-ClassSegmented';

rescaled_ori_jpg_dir = strcat(ori_jpg_dir,'-scaled-',num2str(scale));
mkdir(rescaled_ori_jpg_dir);
fprintf('%s\n',rescaled_ori_jpg_dir);

listing = dir(ori_jpg_dir);
counter = 0;
max_width = 0;% col
max_height = 0;% row
max_rescaled_width = 0;% col
max_rescaled_height = 0;% row
first = true;
for i=1:numel(listing)
	if listing(i).isdir
		continue
	end
	
	%
	counter = counter + 1;
	fprintf('Processing %d/%d\n', counter, numel(listing)-2);% minus 2 for . and ..
    
    jpg_filename = listing(i).name;
	jpg_filepath = strcat(ori_jpg_dir, '/', jpg_filename);
    
    ori_jpg = imread(jpg_filepath);
    rescaled_ori_jpg = imresize(ori_jpg, scale,interpolation_method);

	rescaled_ori_jpg_filename = jpg_filename;
	rescaled_ori_jpg_filepath = strcat(rescaled_ori_jpg_dir, '/', rescaled_ori_jpg_filename);
    
    width = size(ori_jpg,2);
    height = size(ori_jpg,1);
    if width > max_width
        max_width = width;
    end
    if height > max_height
        max_height = height;
    end
       
    rescaled_width = size(rescaled_ori_jpg,2);
    rescaled_height = size(rescaled_ori_jpg,1);
    if rescaled_width > max_rescaled_width
        max_rescaled_width = rescaled_width;
    end
    if rescaled_height > max_rescaled_height
        max_rescaled_height = rescaled_height;
    end
    
	imwrite(rescaled_ori_jpg, rescaled_ori_jpg_filepath);
end

param = [max_width, max_height, max_rescaled_width, max_rescaled_height]';
param_filepath = strcat(rescaled_ori_jpg_dir,'/param.csv');
csvwrite(param_filepath, param);
