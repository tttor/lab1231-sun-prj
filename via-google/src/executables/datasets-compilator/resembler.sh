#this code resembles the existing isolated google images into a google_object list
source ../config
cropped_folder="/home/rizkivmaster/ws/datasets/googleimages/cropped"
echo "Cropped folder:$cropped_folder"
echo "Target google folder: $dpm_img_folder"
echo "Google-Object list: $google_object_file"
echo "Object list file: $object_list_file"

`cut -d , -f 1 $object_list_file > classid`
`cut -d , -f 2 $object_list_file > classname`
imgfolddirs=`ls -d $cropped_folder/*`

`rm imgfolddir`
for dir in $imgfolddirs
do
	echo $dir >> imgfolddir
done

IFS=$'\n' read -d '' -r -a imgfolds < imgfolddir
IFS=$'\n' read -d '' -r -a ids < classid
IFS=$'\n' read -d '' -r -a names < classname
c=0
max=`wc -l < $object_list_file`
echo $max
while [ $c -lt $max ]
do
	imgnames=`ls -d ${imgfolds[$c]}/*`
	for imgname in $imgnames		
	do
		filename=$(basename "$imgname")
		#`cp $imgname $dpm_img_folder/${names[$c]}$filename`
		echo "${ids[$c]},${names[$c]}$filename" >> $google_object_file
	done
	(( c++ ))
done