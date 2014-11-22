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
cd param-generator
./execute.sh
cd ..
# #generate google unaries
cd google-unary-generator
./execute.sh
cd ..
# # #commit F/G segmentation
cd fgannotator
./execute.sh
cd ..
# # #train voc with google/ COMBINATION
cd param-enhancer
./execute.sh
cd ..
# # #generate test unary DTE
cd test-unary-generator
./execute.sh
cd ..
# # #perform multiclass segmentation
cd multiannotation
./execute.sh
cd ..
# #performance evaluation
cd evaluation
./execute.sh
cd ..

cd ../..