% Convert GT png to GT csv 
clear all;
clc;

gt_img_dir_path = '/media/tor/423AF0113AF003A7/tor/robotics/prj/011/dataset/pascal/VOC2010/VOCdevkit/VOC2012/SegmentationClass/';
gt_csv_dir_path = '/media/tor/423AF0113AF003A7/tor/robotics/prj/011/dataset/pascal/VOC2010/VOCdevkit/VOC2012/SegmentationClass-csv/';
listing = dir(gt_img_dir_path);

for i=1:size(listing,1)
	if listing(i).isdir
		continue
	end
	
	% 	
 	gt_img_filename = listing(i).name;
 	gt_img_filepath = strcat(gt_img_dir_path, gt_img_filename);
 	s = sprintf('Processing %s\n', gt_img_filepath);
 	fprintf(s);

 	[img, cmap] = imread(gt_img_filepath);% Somehow for gt png, we do _not_ need to +1 although imwrite() does -1, see http://www.mathworks.com/help/matlab/ref/imwrite.html

 	% 
 	gt_csv_filename = strcat(gt_img_filename(1:size(gt_img_filename,2)-4),'.csv');
 	gt_csv_filepath = strcat(gt_csv_dir_path,gt_csv_filename);
 	csvwrite(gt_csv_filepath, img);
end

% TODO @tttor: write the cmap