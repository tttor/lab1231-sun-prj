source ../config
echo "Trainer path: $train_bin"
echo "Target param folder: $normal_param"
echo "Google Image-Object linker: $google_object_file"
echo "DPM Image folder: $dpm_img_folder"
echo "Google Training List: $google_train_list"
echo "Sampling size:$google_sampling_size"
echo "VOC training folder: $voc_img_folder"
echo "VOC PNG folder: $voc_png_folder"
echo "VOC train list: $voc_train_list"
echo "Normal Param: $normal_param"
echo "##################################"
#relist
`cut -d , -f 1 $google_object_file > objid_tmp`
`cut -d , -f 2 $google_object_file > imgname_tmp`
IFS=$'\n' read -d '' -r -a objid < objid_tmp
IFS=$'\n' read -d '' -r -a imgname < imgname_tmp
`rm objid_tmp`
`rm imgname_tmp`
#initialize sampling bucket
for (( ii = 0; ii < 20; ii++ )); do
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
		if [[ ${quota[${objid[$c]}]} -lt $google_sampling_size ]]; then
			`echo "$singlename" >> $google_train_list`
			quota[${objid[$c]}]=`expr ${quota[${objid[$c]}]} + 1`
		fi
	fi
    (( c++ ))
done

#create google 
`$train_bin $voc_train_list  $voc_img_folder $voc_png_folder $normal_param`


