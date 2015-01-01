%make_csv_from_png.m

function cmap = make_csv_from_png_voc(png_filepath, csv_filepath)
	%
 	[img, cmap] = imread(png_filepath);    

 	% 
 	csvwrite(csv_filepath, img);
end