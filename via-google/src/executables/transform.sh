#download google images
# cd google-retriever
# ./execute.sh
# cd ..
# cd list-and-extract
# ./execute.sh
# cd ..
#resize to fit voc
# cd list-and-resize
# ./execute.sh
# cd ..
# #train initial parameters
# cd param-generator
# ./execute.sh
# cd ..
# #generate google unaries
# cd google-unary-generator
# ./execute.sh
# cd ..
# #commit F/G segmentation
# cd fgannotator
# ./execute.sh
# cd ..
# #train voc with google
cd param-enhancer
./execute.sh
cd ..
# #generate test unary
# cd test-unary-generator
# ./execute.sh
# cd ..
# #perform multiclass segmentation
# cd multiannotator
# ./execute.sh
# cd ..
# #performance evaluation
# cd evaluation
# ./execute.sh
# cd ..
