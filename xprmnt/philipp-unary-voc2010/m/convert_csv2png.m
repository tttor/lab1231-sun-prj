clear all;
clc;

%% change this path if you install the VOC code elsewhere
VOCcode_dir = '/home/tor/sun3/dataset/pascal/VOC2012/VOCdevkit/VOCcode';
addpath(VOCcode_dir);

%%
csv_dirpath = '/media/tor/423AF0113AF003A7/tor/robotics/prj/011/dataset/pascal/VOC2012/VOCdevkit/VOC2012/SegmentationClass-csv';
png_dirpath = '/home/tor/sun4/xprmnt/philipp-unary-voc2010/result/all_val_cls';

% The size of the color map is given by N, which should generally be set to 256 to include a color for the 'void' label.
cmap = VOClabelcolormap(256);

listing = dir(csv_dirpath);
counter = 0;
for i=1:numel(listing)
	if listing(i).isdir
		continue
	end
	
	%
	counter = counter + 1;
	fprintf('Processing %d/%d\n', counter, numel(listing)-2);

 	csv_filename = listing(i).name;
 	csv_filepath = strcat(csv_dirpath, '/', csv_filename);

 	png_filename = strcat(csv_filename(1:numel(csv_filename)-4), '.png');
	png_filepath = strcat(png_dirpath, '/', png_filename);

	make_png_from_csv(csv_filepath, png_filepath, cmap) 	
end