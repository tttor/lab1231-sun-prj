source ../config
echo "Target model name list file:"
echo "$classlistfile"
echo "Target model directory:"
echo "$modelfolder"
echo "Target images folders per class directory:"
echo "$googleimgfolder"
echo "Target cropped image folder:"
echo "$targetcropfolder"
echo "Target xml folder"
echo "$targetxmlfolder"

echo "######################"

classlist=`cat $classlistfile`


modeldirs=`ls -d $modelfolder/*`


imgfolddirs=`ls -d $googleimgfolder/*`


# for class in $classlist
#  do
#  	echo $class
#  done
`rm modeldir`
 for model in $modeldirs
 do
 	echo $model >> modeldir
 	echo $model
 done

`rm imgfolddir`
for dir in $imgfolddirs
do
	echo $dir >> imgfolddir
	echo $dir
done

IFS=$'\n' read -d '' -r -a imgfolds < imgfolddir
IFS=$'\n' read -d '' -r -a classes < $classlistfile
IFS=$'\n' read -d '' -r -a models < modeldir




`rm compilation`
c=0
max=`wc -l < $classlistfile`
echo $max
while [ $c -lt $max ]
do
	`rm ok`
	`rm error`
	`find ${imgfolds[$c]}/ -name '*.jpg' -exec identify -format "%f" {} \; 1>${imgfolds[$c]}/ok 2>${imgfolds[$c]}/errors`
	`find ${imgfolds[$c]}/ -name '*.png' -exec identify -format "%f" {} \; 1>>${imgfolds[$c]}/ok 2>>${imgfolds[$c]}/errors`
	imgnames=`cat ${imgfolds[$c]}/ok`
	for imgname in $imgnames
	do
		echo "$c,${classes[$c]},${models[$c]},${imgfolds[$c]}/$imgname,$targetcropfolder/${classes[$c]},$targetxmlfolder/${classes[$c]}" >> compilation
		echo "$c,${classes[$c]},${models[$c]},${imgfolds[$c]}/$imgname"
	done
	`rm ok`
	`rm error`
	(( c++ ))
done
`rm imgfolddir`
`rm modeldir`

