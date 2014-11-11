echo "Target model name list file:"
echo "$1"
echo "Target model directory:"
echo "$2"
echo "Target images folders per class directory:"
echo "$3"
echo "Model Size"
echo `wc -l $1`
echo "######################"

classlist=`cat $1`


modeldirs=`ls -d $2/*`


imgfolddirs=`ls -d $3/*`


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
IFS=$'\n' read -d '' -r -a classes < $1
IFS=$'\n' read -d '' -r -a models < modeldir




`rm compilation`
c=0
max=`wc -l < $1`
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
		echo "$c,${classes[$c]},${models[$c]},${imgfolds[$c]}/$imgname,$4/${classes[$c]},$5/${classes[$c]}" >> compilation
		echo "$c,${classes[$c]},${models[$c]},${imgfolds[$c]}/$imgname"
	done
	`rm ok`
	`rm error`
	(( c++ ))
done
`rm imgfolddir`
 # echo `wc -l modeldir`

# echo "${classes[0]}"
# echo "${models[0]}"

`rm modeldir`
