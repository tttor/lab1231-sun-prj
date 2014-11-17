source ../config
target=$googleimgfolder
echo "Target cropped image folder:"
echo "$target"
echo "######################"


imgfolddirs=`ls -d $target/*`

`rm imgfolddir`
for dir in $imgfolddirs
do
	echo $dir >> imgfolddir
	#echo $dir
done

IFS=$'\n' read -d '' -r -a imgfolds < imgfolddir

#`rm allclasssmrf`

c=0
#max=`wc -l < $1`
max=1
echo $max
while [ $c -le $max ]
do
	#`rm ${names[$c]}smrf`
	imgnames=`ls -d ${imgfolds[$c]}/*`
	for imgname in $imgnames		
	do
		mogrify -resize '500x500>' $imgname
	done
	(( c++ ))
done
`rm imgfolddir`