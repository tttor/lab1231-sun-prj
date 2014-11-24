#this will automatically compile all codes
# and manage experiments throughly

cd build
make
cd ..

cd src/textonboost/build
make
cd ../../..

cd src/executables
source config
`cp config $current_exp`
`cp $settings_path $current_exp`

logpath="$current_exp/log$RANDOM"


echo "START TRANSFORM" > $logpath
date +"%m-%d-%y %T" >> $logpath


#download google images
# cd google-retriever
# ./execute.sh
# cd ..
#
# cd list-and-extract
# ./execute.sh
# cd ..
#resize to fit voc
# cd list-and-resize
# ./execute.sh
# cd ..
# #train initial parameters START HERE

echo "START GENERATING INITIAL MODEL" >> $logpath
date +"%m-%d-%y %T" >> $logpath

cd param-generator
./execute.sh
cd ..
# #generate google unaries

echo "START GENERATING GOOGLE UNARY" >> $logpath
date +"%m-%d-%y %T" >> $logpath

cd google-unary-generator
./execute.sh
cd ..
# # #commit F/G segmentation

echo "START F/G SEGMENTATION" >> $logpath
date +"%m-%d-%y %T" >> $logpath

cd fgannotator
./execute.sh
cd ..
# # #train voc with google/ COMBINATION

echo "START IMPROVING MODEL" >> $logpath
date +"%m-%d-%y %T" >> $logpath

cd param-enhancer
./execute.sh
cd ..
# # #generate test unary DTE
echo "START TEST UNARY GENERATION" >> $logpath
date +"%m-%d-%y %T" >> $logpath

cd test-unary-generator
./execute.sh
cd ..
# # #perform multiclass segmentation
echo "START TESTSET ANNOTATION" >> $logpath
date +"%m-%d-%y %T" >> $logpath

cd multiannotation
./execute.sh
cd ..

# #performance evaluation

echo "START EVALUATION" >> $logpath
date +"%m-%d-%y %T" >> $logpath

cd evaluation
./execute.sh
cd ..

cd ../..