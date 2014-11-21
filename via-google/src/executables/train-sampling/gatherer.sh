source ../config
echo "Target class list file:"
echo "$classlistfile"
echo "Images per Class size:"
echo "$classSize"
echo "Source images folders per class directory:"
echo "$googleimgfolder"
echo "Target training images folders:"
echo "$trainfolder"
echo "Target Test.txt folder directory:"
echo "$splitfolder"
echo "######################"

classlist=`cat $classlistfile`

imgfolddirs=`ls -d $googleimgfolder/*`


# for class in $classlist
#  do
#  	echo $class
#  done
`rm imgfolddir`
echo "Retrieving folder list"
for dir in $imgfolddirs
do
	echo $dir >> imgfolddir
	echo $dir
done

IFS=$'\n' read -d '' -r -a imgfolds < imgfolddir
IFS=$'\n' read -d '' -r -a classes < $classlistfile




#`rm compilation`
`rm $splitfolder/Test.txt`
c=0
limit=$classSize
#half=10
#`expr $limit / 2`
max=`wc -l < $classlistfile`
echo $max
while [ $c -lt $max ]
do
	counter=0
	imgnames=`ls -d ${imgfolds[$c]}/*`
	for imgname in $imgnames
	do
		if [ $counter -lt $limit ]
		then
			filename=$(basename "$imgname")
			extension="${filename##*.}"
			singlename="${filename%.*}"
			`cp $imgname $trainfolder/${classes[$c]}$filename`
			echo "${classes[$c]}$singlename" >> $splitfolder/Test.txt
		else
			break
		fi
		(( counter++ ))
	done
	(( c++ ))
done
`rm imgfolddir`
