source ../config
echo "Target cropped image folder:"
echo "$dpm_img_folder"
echo "######################"


imgfolddirs=`ls -d $dpm_img_folder/*`

for path in $imgfolddirs
do
	mogrify -resize '500x500>' $path
done
