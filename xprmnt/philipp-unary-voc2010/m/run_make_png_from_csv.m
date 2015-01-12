%%
clear all;
clc;

util_dir = '../../../util/src-m';
addpath(util_dir);

VOCcode_dir = '../../../external/VOCdevkit/VOCcode';
addpath(VOCcode_dir);

%%
csv_dir = '/media/tor/46DAF35ADAF344A9/tor/robotics/prj/011/xprmnt/generating-philipp-unary-voc2010/unary-scaled/annotation/split_voc2010_philipp_Test_csv';
png_dir = '/media/tor/46DAF35ADAF344A9/tor/robotics/prj/011/xprmnt/generating-philipp-unary-voc2010/unary-scaled/annotation/split_voc2010_philipp_Test_cls';
mkdir(png_dir);

%%
% The VOClabelcolormap function creates the color map which has been used for all provided indexed images. 
% You should use this color map for writing your own indexed images, for consistency. 
% The size of the color map is given by N, which should generally be set to 256 to include a color for the `void' label.
cmap = VOClabelcolormap(256);

%%
listing = dir(csv_dir);
counter = 0;
for i=1:numel(listing)
	if listing(i).isdir
		continue
	end
	
	% 	
	counter = counter + 1;
	fprintf('Processing %d/%d\n', counter, numel(listing)-2);

	csv_filename = listing(i).name;
	csv_filepath = strcat(csv_dir, '/', csv_filename);

	png_filename = strcat(csv_filename(1:numel(csv_filename)-4), '.png');
	png_filepath = strcat(png_dir, '/', png_filename);

	% The size of the color map is given by N, which should generally be set to 256 to include a color for the 'void' label.
	make_png_from_csv_voc(csv_filepath, png_filepath, cmap)    
end
