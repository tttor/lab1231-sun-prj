source ../config
echo "Annotator path:"
echo "$vocannbinpath"
echo "Image List"
echo "$testlistfile"
echo "Image Sources folder:"
echo "$trainfolder"
echo "Source unaries folder:"
echo "$unaryfolder"
echo "Annotation CSV folder:"
echo "$anncsvfolder"
echo "Target Annotation Image folder"
echo "$segclassfolder"
echo "######################"

imglist=`cat $testlistfile`

for imgname in $imglist
do
	`$vocannbinpath $datasettype $trainfolder/$imgname.jpg $unaryfolder/$imgname.unary $anncsvfolder/$imgname.ann $segclassfolder/$imgname.png`
done

