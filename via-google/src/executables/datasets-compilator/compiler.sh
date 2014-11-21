#This file compiles the instruction for SMRF creator and

echo "Target parameter file:"
echo "$1"
echo "Target cropped image folder:"
echo "$2"
echo "Target annotation folder:"
echo "$3"
echo "Target annotator file:"
echo "$4"
echo "Target training folder"
echo "$5"
echo "Target unary folder"
echo "$6"
echo "######################"


`cut -d , -f 1 $1 > classid`
`cut -d , -f 2 $1 > classname`
`cut -d , -f 3 $1 > classalpha`
`cut -d , -f 4 $1 > classbeta`

imgfolddirs=`ls -d $2/*`

`rm imgfolddir`
for dir in $imgfolddirs
do
	echo $dir >> imgfolddir
done

IFS=$'\n' read -d '' -r -a imgfolds < imgfolddir
IFS=$'\n' read -d '' -r -a ids < classid
IFS=$'\n' read -d '' -r -a names < classname
IFS=$'\n' read -d '' -r -a alphas < classalpha
IFS=$'\n' read -d '' -r -a betas < classbeta


#`rm allclasssmrf`

`rm Train.txt`
c=0
max=`wc -l < $1`
echo $max
while [ $c -le $max ]
do
	#`rm ${names[$c]}smrf`
	`mkdir $3/${names[$c]}`
	imgnames=`ls -d ${imgfolds[$c]}/*`
	for imgname in $imgnames		
	do
		filename=$(basename "$imgname")
		#create list of training file and copy to target training folder
		`cp $imgname $5/${names[$c]}$filename`
		echo "${names[$c]}$filename" >> Train.txt
	done
	(( c++ ))
done

`rm pnglist`
c=0
max=`wc -l < $1`
echo $max
while [ $c -le $max ]
do
	#`rm ${names[$c]}smrf`
	`mkdir $3/${names[$c]}`
	imgnames=`ls -d ${imgfolds[$c]}/*`
	for imgname in $imgnames		
	do
		filename=$(basename "$imgname")
		extension="${filename##*.}"
		singlename="${filename%.*}"
		#echo "$4 $imgname $3/${names[$c]}/$filename.png ${alphas[$c]} ${betas[$c]} ${ids[$c]}" #>> ${names[$c]}smrf`

		#create list of training file and copy to target training folder
		`cp $imgname $5/${names[$c]}$filename`
		echo "${names[$c]}$filename" >> Train.txt

		#`$4 $imgname $3/${names[$c]}/$filename.png ${alphas[$c]} ${betas[$c]} ${ids[$c]}`
		echo "$3/${names[$c]}/$filename.png" >> pnglist
	done
	(( c++ ))
done
