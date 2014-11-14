classListPath="/home/rizkivmaster/ws/lab1231-sun-prj/via-google/src/executables/classlist"
srcDir="/home/rizkivmaster/ws/datasets/googleimages/cropped"
tgtDir="/home/rizkivmaster/ws/TextonBoost/voc/datasets/JPEGImages"
splitDir="/home/rizkivmaster/ws/TextonBoost/voc/split"
quota=60
blockSize=10
echo "Target class list file:"
echo "$classListPath"
echo "Source images folders per class directory:"
echo "$srcDir"
echo "Target training images folders:"
echo "$tgtDir"
echo "Target Test.txt folder directory:"
echo "$splitDir"
echo "Size of Quota:"
echo "$quota"
echo "Size of Block Size:"
echo "$blockSize"
echo "######################"



classlist=`cat $classListPath`

imgfolddirs=`ls -d $srcDir/*`


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
IFS=$'\n' read -d '' -r -a classes < $classListPath




`rm $splitDir/Test.txt`
`rm tempList`
c=0
trainIndex=1
limit=60
half=10
#`expr $limit / 2`
max=`wc -l < $classListPath`
echo $max
while [ $c -lt $max ]
do
	counter=0
	imgnames=`ls -d ${imgfolds[$c]}/*`
	for imgname in $imgnames
	do
		if [ $counter -lt $quota ]
		then
			filename=$(basename "$imgname")
			extension="${filename##*.}"
			singlename="${filename%.*}"
			`cp $imgname $tgtDir/${classes[$c]}$filename`
			echo "${classes[$c]}$singlename" >> tempList
		else
			break
		fi
		(( counter++ ))
	done
	(( c++ ))
done


listFile=`cat tempList`
blockIndex=1
counter=0
for file in listFile
do
	if [ $counter -lt $blockSize ]
		then
		echo "$file" >> $splitDir/Test$blockIndex.txt
		(( counter++ ))
	else
		counter=0
		(( blockSize++ ))
	fi
done

`rm imgfolddir`
`rm tempList`