source ..config
echo "Evaluator: $evaluator_path"
echo "GT CSV: $voc_gt_csv"
echo "Test CSV: $google_csv_dir"
echo "Google Image-Object linker: $google_object_file"

#relist
`cut -d , -f 1 $google_object_file > objid_tmp`
`cut -d , -f 2 $google_object_file > imgname_tmp`
IFS=$'\n' read -d '' -r -a objid < objid_tmp
IFS=$'\n' read -d '' -r -a imgname < imgname_tmp
`rm objid_tmp`
`rm imgname_tmp`
`rm eval_tmp`

image_size=${#imgname[@]}
c=0
while [ $c -lt $image_size ]
do
	filename=$(basename "${imgname[$c]}")
	extension="${filename##*.}"
	singlename="${filename%.*}"
	if [[ -f "$google_csv_dir/$singlename.csv" ]]; then
			`echo "$singlename.csv" >> eval_tmp`
	fi
    (( c++ ))
done

`python $evaluator_path $voc_gt_csv $google_csv_dir ${PWD}/eval_tmp`
`rm eval_tmp`