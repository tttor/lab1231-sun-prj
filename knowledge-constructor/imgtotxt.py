# include related library

import os
import glob
from PIL import Image
import voc_obj_intolbl
import voc_obj_lbltoin
import spa_intopos
import math

# insert label	
indexToNameDict=voc_obj_intolbl.LABELS
nameToIndexDict=voc_obj_lbltoin.LABELS

#dataset to be read
#cooccurFile=open("csv/cooccurrenceFreq.csv","w") 
cooccurFile=open("csv/coocTest.csv","w") 				# Test CSV
imgTxt=open("test.csv","w")



############################################# CO-OCCURRENCE IMAGE PROCESSING FUNCTION ##################################################

#path='/home/ian-djakman/Documents/data/voc_dataset/VOCdevkit/VOC2012/SegmentationClass'
path='img/' #local test path
fileQuant = glob.glob(os.path.join(path, '*.png'))
total=len(fileQuant)

for (num, infile) in enumerate(fileQuant):
	datasetImage = Image.open(infile)
	pixel        = datasetImage.load()
	imageSize    = datasetImage.size
	(imageWidth, imageHeight)  = imageSize
	
	print("Processing " + str(num+1) +"/" + str(total) + " files for Coocurrence", end="\r") # Proccessing Progress Feedback
	




####################################################### PRINTING ####################################################################

#Tulis Pengubah Image ke File
print("")
imgTxt.write("%d,"%imageWidth)
imgTxt.write("%d,"%imageHeight)
imgTxt.write("\n")
for i in range(imageWidth):
	for j in range(imageHeight):
		imgTxt.write("%d,"%i)
		imgTxt.write("%d,"%j)
		imgTxt.write("%d,"%pixel[i,j])
		imgTxt.write("\n")


