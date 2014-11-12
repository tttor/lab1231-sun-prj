echo "Target class list file:"
echo "$1"
echo "Source images folders per class directory:"
echo "$2"
echo "Target training images folders:"
echo "$3"
echo "Target Test.txt folder directory:"
echo "$4"
echo "######################"

classlist=`cat $1`

imgfolddirs=`ls -d $2/*`


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
IFS=$'\n' read -d '' -r -a classes < $1




#`rm compilation`
`rm $4/Test.txt`
c=0
limit=60
#max=`wc -l < $1`
max=1
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
			`cp $imgname $3/${classes[$c]}$filename`
			# "$c,$4/${classes[$c]}/$imgname,$6/${classes[$c]}/${filename}_GT.png" >> training.txt
			# else
			# 	echo "$4/${classes[$c]}/$imgname does not exist"
			# fi
			# #echo "$c,${classes[$c]},${models[$c]},${imgfolds[$c]}/$imgname,$4/${classes[$c]}/$imgname,$5/${classes[$c]}/filename.xml" >> compilation
			# echo "$c,${classes[$c]},${models[$c]},${imgfolds[$c]}/$imgname"
			echo "${classes[$c]}$filename" >> $4/Test.txt
		else
			break
		fi
		(( counter++ ))
	done
	(( c++ ))
done
`rm imgfolddir`