% make_png_from_csv.m

function make_png_from_csv_voc(csv_filepath, png_filepath, cmap)
	ann = csvread(csv_filepath);
	
	% Plus one as imwrite() does -1 when making the target png, 
    % see http://www.mathworks.com/help/matlab/ref/imwrite.html
    ann = ann + 1;

	%
	imwrite(ann, cmap,png_filepath);
end