clear all;
clc;

util_dir = '/home/tor/lab1231-sun-prj/util/src-m';
addpath(util_dir);

VOCcode_dir = '/home/tor/dataset/pascal/VOC2010/VOCdevkit/VOCcode';
addpath(VOCcode_dir);

%%
gt_png_dirpath = '/home/tor/dataset/pascal/VOC2010/VOCdevkit/VOC2010/SegmentationClass-regenerated-from-csv';
gt_csv_dirpath = '/home/tor/dataset/pascal/VOC2010/VOCdevkit/VOC2010/SegmentationClass-csv';
mkdir(gt_png_dirpath);

%%
% The VOClabelcolormap function creates the color map which has been used for all provided indexed images. 
% You should use this color map for writing your own indexed images, for consistency. 
% The size of the color map is given by N, which should generally be set to 256 to include a color for the `void' label.
cmap = VOClabelcolormap(256);

listing = dir(gt_csv_dirpath);
counter = 0;
for i=1:numel(listing)
	if listing(i).isdir
		continue
	end
	
	%
	counter = counter + 1;
	fprintf('Processing %d/%d\n', counter, numel(listing)-2);

    csv_filename = listing(i).name;
	csv_filepath = strcat(gt_csv_dirpath, '/', csv_filename);
    
	png_filename = strcat(csv_filename(1:numel(csv_filename)-4), '.png');
	png_filepath = strcat(gt_png_dirpath, '/', png_filename);
	
	make_png_from_csv_voc(csv_filepath, png_filepath, cmap);
end