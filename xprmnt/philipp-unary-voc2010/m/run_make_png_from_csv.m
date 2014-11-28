%%
clear all;
clc;

util_dir = '../../../util/src-m';
addpath(util_dir);

VOCcode_dir = '../../../external/VOCdevkit/VOCcode';
addpath(VOCcode_dir);

%%
csv_dir = '/home/tor/sun4/xprmnt/philipp-unary-voc2010/result/merged_test_csv';
png_dir = '/home/tor/sun4/xprmnt/philipp-unary-voc2010/result/merged_test_cls';

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
	cmap = VOClabelcolormap(256);
	make_png_from_csv_voc(csv_filepath, png_filepath, cmap)
end
