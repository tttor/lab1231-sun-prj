load('palette.mat');
file = fopen('pnglist');
C = textscan(file,'%s','Delimiter',',');
dirs = C{1};
for idx = 1:size(dirs,1)
    try
    dir = char(dirs{idx})
    tgtimg = imread(dir);
    imwrite(tgtimg,palette,dir,'png');
    catch
        fprintf(['skip' dir]);
    end
end