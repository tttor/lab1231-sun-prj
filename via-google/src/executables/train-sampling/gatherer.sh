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
half=10
#`expr $limit / 2`
max=`wc -l < $1`
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
			if [ $counter -lt $half ]
			then
				echo "${classes[$c]}$singlename" >> $4/Validation.txt
			else
				echo "${classes[$c]}$singlename" >> $4/Test.txt
			fi
		else
			break
		fi
		(( counter++ ))
	done
	(( c++ ))
done
`rm imgfolddir`