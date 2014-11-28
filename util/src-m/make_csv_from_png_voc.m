%make_csv_from_png.m

function cmap = make_csv_from_png_voc(png_filepath, csv_filepath)
	%
 	[img, cmap] = imread(png_filepath);

 	% Need to +1 since imwrite() does -1 when making the source png, see http://www.mathworks.com/help/matlab/ref/imwrite.html
 	img = img + 1;

 	% 
 	csvwrite(csv_filepath, img);
end