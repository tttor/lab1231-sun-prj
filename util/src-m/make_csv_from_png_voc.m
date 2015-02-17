%make_csv_from_png.m

function cmap = make_csv_from_png_voc(png_filepath, csv_filepath)
	%
 	[img, cmap] = imread(png_filepath);    

    % SOMEHOW, THIS MODIFICATION IS NOT NEEDED
    % modifies the indexed image on pixels containing obj-class idx
    % void_idx=255 is not modified
    % because matlab indexing begin at 1, while at python at 0
%     void_idx = 255;
%     objclass_idxes = find(img < void_idx);
%     img(objclass_idxes) = img(objclass_idxes) - 1;
    
 	% 
 	csvwrite(csv_filepath, img);      
end