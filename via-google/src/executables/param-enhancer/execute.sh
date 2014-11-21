source ../config
echo "Trainer path: $train_bin"
echo "Target param folder: $improved_param"
echo "Google Image-Object linker: $google_object_file"
echo "DPM Image folder: $dpm_img_folder"
echo "Google PNG folder: $google_png_dir"

echo "VOC training folder: $voc_img_folder"
echo "VOC PNG folder: $voc_png_folder"
echo "VOC train list: $voc_train_list"
echo "##################################"
#relist
`cut -d , -f 1 $google_object_file > objid_tmp`
`cut -d , -f 2 $google_object_file > imgname_tmp`
IFS=$'\n' read -d '' -r -a objid < objid_tmp
IFS=$'\n' read -d '' -r -a imgname < imgname_tmp
`rm objid_tmp`
`rm imgname_tmp`
#initialize sampling bucket
for (( ii = 0; ii < 21; ii++ )); do
	quota[$ii]=0
done

image_size=${#imgname[@]}
c=0
while [ $c -lt $image_size ]
do
	filename=$(basename "${imgname[$c]}")
	extension="${filename##*.}"
	singlename="${filename%.*}"
	if [[ -f "$dpm_img_folder/$singlename.jpg" ]]; then
		if [[ -f "$google_png_dir/$singlename.png" ]]; then
			`echo "$singlename" >> $combined_train_list`
			`cp $dpm_img_folder/$singlename.jpg $voc_img_folder/$singlename.jpg`
			`cp $google_png_dir/$singlename.jpg $voc_png_folder/$singlename.png`
		fi
	fi
    (( c++ ))
done

`cat $voc_train_list >> $combined_train_list`

#create google 
`$train_bin $combined_train_list  $voc_img_folder $voc_png_folder $improved_param`


