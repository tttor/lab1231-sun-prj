source ../config
echo "Annotator path:"
echo "$googleannbinpath"
echo "$googlevocannbinpath"
echo "Target class list file:"
echo "$classlistfile"
echo "Images per Class size:"
echo "$classSize"
echo "Source images folders per class directory:"
echo "$googleimgfolder"
echo "Source unaries folder:"
echo "$unaryfolder"
echo "Annotation CSV folder:"
echo "$anncsvfolder"
echo "Target Annotation Image folder"
echo "$segclassfolder"
echo "######################"

classlist=`cat $classlistfile`
datasettype="voc"
ann
imgfolddirs=`ls -d $googleimgfolder/*`


# for class in $classlist
#  do
#  	echo $class
#  done
`rm imgfolddir`
`rm pnglist`
echo "Retrieving folder list"
for dir in $imgfolddirs
do
	echo $dir >> imgfolddir
	echo $dir
done

IFS=$'\n' read -d '' -r -a imgfolds < imgfolddir
IFS=$'\n' read -d '' -r -a classes < $classlistfile

c=0
limit=$classSize
max=`wc -l < $classlistfile`
classindex=1
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
			if[ anntype="voc"]
				then
				`$googleannbinpath $datasettype $imgname $unaryfolder/$singlename.unary $anncsvfolder/$singlename.ann $segclassfolder/$singlename.png`
			fi
			if[ anntype="googlevoc"]
				then
				`$googlevocbinpath $datasettype $imgname $unaryfolder/$singlename.unary $anncsvfolder/$singlename.ann $segclassfolder/$singlename.png`
			fi
			echo "$segclassfolder/$singlename.png" >> pnglist

		else
			break
		fi
		(( counter++ ))
		(( classindex++ ))
	done
	(( c++ ))
done
`rm imgfolddir`

matlab -nodesktop -nosplash -r "paletting;quit;"
`rm pnglist`