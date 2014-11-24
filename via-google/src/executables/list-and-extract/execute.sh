source ../config

echo "Object list file:"
echo "$object_list_file"

echo "Object Google list file:"
echo "$google_object_file"

echo "OBJECT-DPM linker file:"
echo "$object_dpm_file"

echo "Google Image Source"
echo "$google_folder"


echo "Target cropped image folder:"
echo "$dpm_img_folder"

echo "Target xml folder"
echo "$dpm_xml_folder"

echo "######################"
`rm compilation`
`cut -d , -f 1 $object_dpm_file > objid_tmp`
`cut -d , -f 2 $object_dpm_file > dpmname_tmp`
IFS=$'\n' read -d '' -r -a objid < objid_tmp
IFS=$'\n' read -d '' -r -a dpmname < dpmname_tmp
`rm objid_tmp`
`rm dpmname_tmp`

#mapping
dpm_size=${#objid[@]}
c=0
while [ $c -lt $dpm_size ]
do
	dpm[${objid[$c]}]=${dpmname[$c]}
	(( c++ ))
done
############################################

`cut -d , -f 1 $object_list_file > objid_tmp`
`cut -d , -f 2 $object_list_file > objname_tmp`
IFS=$'\n' read -d '' -r -a objid < objid_tmp
IFS=$'\n' read -d '' -r -a objname < objname_tmp
`rm objid_tmp`
`rm objname_tmp`

#mapping
obj_size=${#objid[@]}
c=0
while [ $c -lt $obj_size ]
do
	classname[${objid[$c]}]=${objname[$c]}
	(( c++ ))
done
########################################################
#create cache
`cut -d , -f 1 $google_object_file > objid_tmp`
`cut -d , -f 2 $google_object_file > imgname_tmp`
IFS=$'\n' read -d '' -r -a objid < objid_tmp
IFS=$'\n' read -d '' -r -a imgname < imgname_tmp
`rm objid_tmp`
`rm imgname_tmp`

image_size=${#imgname[@]}
c=0
while [ $c -lt $image_size ]
do
	filename=$(basename "${imgname[$c]}")
	extension="${filename##*.}"
	singlename="${filename%.*}"
	if [[ $(identify -format '%f' "$google_folder/${imgname[$c]}" 2>/dev/null) == ${imgname[$c]} ]]; then
        #if ok
        if [ $extension == "jpg" ] || [ $extension == "JPG"  ]; then
        	#id object, nama file dpm, source path, target img path, target xml path
        	echo "${objid[$c]},${classname[${objid[$c]}]},${dpm[${objid[$c]}]},$google_folder/${imgname[$c]},$dpm_img_folder/$singlename.jpg,$dpm_xml_folder/$singlename.xml" >> compilation
    	fi
    else
    	#if not ok
    	echo "${imgname[$c]} is corrupted"
    fi
    (( c++ ))
done

/usr/local/MATLAB/R2012a/bin/matlab -nodesktop -nosplash -r "extract();quit;"

echo "Image isolation is finished"
`rm compilation`