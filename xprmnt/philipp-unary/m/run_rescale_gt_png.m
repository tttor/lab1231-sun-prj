clear all;
clc;

set_rescaling_param;
gt_png_dir = '/home/tor/sun3/dataset/pascal/VOC2010/VOCdevkit/VOC2010/SegmentationClass';

rescaled_gt_png_dir = strcat(gt_png_dir,'-scaled-',num2str(scale));
mkdir(rescaled_gt_png_dir);
fprintf('%s\n',rescaled_gt_png_dir);

listing = dir(gt_png_dir);
counter = 0;
first = true;
old_rescaled_map = zeros(0,0);% a dummy init value
for i=1:numel(listing)
	if listing(i).isdir
		continue
	end
	
	%
	counter = counter + 1;
	fprintf('Processing %d/%d\n', counter, numel(listing)-2);% minus 2 for . and ..
    
    png_filename = listing(i).name;
	png_filepath = strcat(gt_png_dir, '/', png_filename);
    
    [gt_png, map] = imread(png_filepath);
    [rescaled_gt_png, rescaled_map] = imresize(gt_png, map, scale,interpolation_method,'Colormap','original');

	rescaled_png_filename = png_filename;
	rescaled_png_filepath = strcat(rescaled_gt_png_dir, '/', rescaled_png_filename);
    
    if ~first
        assert(old_rescaled_map==rescaled_map,'old_rescaled_map!=rescaled_map')
    end
    old_rescaled_map = rescaled_map;
    
	imwrite(rescaled_gt_png, rescaled_map, rescaled_png_filepath)    
end

%color_map_filepath = strcat(rescaled_gt_png_dir,'/color_map.csv');
%csvwrite(color_map_filepath, old_rescaled_map);