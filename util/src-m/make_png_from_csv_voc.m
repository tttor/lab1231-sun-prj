% make_png_from_csv.m

function make_png_from_csv_voc(csv_filepath, png_filepath, cmap)
	ann = csvread(csv_filepath);
    
    % SOMEHOW, THIS MODIFICATION IS NOT NEEDED
%     % modifies the annotation on pixels containing obj-class idx
%     % void_idx=255 is not modified
%     % because matlab indexing begin at 1, while at python at 0
%     void_idx = 255;
%     objclass_idxes = find(ann < void_idx);
%     ann(objclass_idxes) = ann(objclass_idxes) + 1;
    	
	% Plus one as imwrite() does -1 when making the target png, 
    % see http://www.mathworks.com/help/matlab/ref/imwrite.html
    ann = ann + 1;

	%
	imwrite(ann, cmap,png_filepath);
end