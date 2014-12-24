clear all;
clc;

util_dir = '/home/tor/sun/ws/lab1231-sun-prj/util/src-m';
addpath(util_dir);

VOCcode_dir = '/home/tor/sun3/dataset/pascal/VOC2012/VOCdevkit/VOCcode';
addpath(VOCcode_dir);

%%
gt_png_dirpath = '/home/tor/sun3/dataset/pascal/VOC2010/VOCdevkit/VOC2010/SegmentationClass-scaled-0.25';
gt_csv_dirpath = '/home/tor/sun3/dataset/pascal/VOC2010/VOCdevkit/VOC2010/SegmentationClass-csv-0.25';
mkdir(gt_csv_dirpath);

%%
listing = dir(gt_png_dirpath);
counter = 0;
for i=1:numel(listing)
	if listing(i).isdir
		continue
	end
	
	%
	counter = counter + 1;
	fprintf('Processing %d/%d\n', counter, numel(listing)-2);

	png_filename = listing(i).name;
	png_filepath = strcat(gt_png_dirpath, '/', png_filename);

	csv_filename = strcat(png_filename(1:numel(png_filename)-4), '.csv');
	csv_filepath = strcat(gt_csv_dirpath, '/', csv_filename);

	make_csv_from_png_voc(png_filepath, csv_filepath);
end