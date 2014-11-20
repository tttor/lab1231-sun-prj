#download google images
cd google-retriever
./execute.sh
cd ..
#isolate images
cd list-and-extract
./execute.sh
cd ..
#resize to fit voc
cd list-and-resize
./execute.sh
cd ..
#train initial parameters
cd param-generator
./execute.sh
cd ..
#generate google unaries
cd google-unary-generator
./execute.sh
cd ..
#commit F/G segmentation WE ARE STILL HERE
cd fgannotator
./execute.sh
cd ..
#train voc with google
cd param-enhancer
./execute.sh
cd ..
#generate test unary
cd test-generator
./execute.sh
cd ..
#perform multiclass segmentation
cd multiannotator
./execute.sh
cd ..
#performance evaluation
cd evaluation
./execute.sh
cd ..
#CAN WE EVALUATE PNG ONLY?
#IF YES: USE ONLY PNG
#IF NO: USING ANN
	#CONVERT PNG TO ANN
	#EVALUATE