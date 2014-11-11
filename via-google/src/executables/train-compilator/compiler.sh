#This file compiles the instruction for SMRF creator and

echo "Target parameter file:"
echo "$1"
echo "Target cropped image folder:"
echo "$2"
echo "Target training folder"
echo "$3"
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
	#echo $dir
done

IFS=$'\n' read -d '' -r -a imgfolds < imgfolddir
IFS=$'\n' read -d '' -r -a ids < classid
IFS=$'\n' read -d '' -r -a names < classname
IFS=$'\n' read -d '' -r -a alphas < classalpha
IFS=$'\n' read -d '' -r -a betas < classbeta


#`rm allclasssmrf`

`rm Test.txt`
c=0
max=`wc -l < $1`
echo $max
while [ $c -le $max ]
do
	imgnames=`ls -d ${imgfolds[$c]}/*`
	for imgname in $imgnames		
	do
		filename=$(basename "$imgname")
		singlename="${filename%.*}"
		#create list of training file and copy to target training folder
		`cp $imgname $3/${names[$c]}$filename`
		echo "${names[$c]}$singlename" >> Test.txt
	done
	(( c++ ))
done

`rm classid`
`rm classname`
`rm classalpha`
`rm classbeta`
`rm imgfolddir`