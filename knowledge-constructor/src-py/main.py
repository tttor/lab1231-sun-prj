# LIBRARY
import os
import glob
from PIL import Image
import voc_obj_intolbl
import voc_obj_lbltoin
import spa_intopos
import math
import json
import metaConstructor
import knowledgeConstructor

# DICTIONARY
indexToNameDict=voc_obj_intolbl.LABELS
nameToIndexDict=voc_obj_lbltoin.LABELS
indexToPosDict=spa_intopos.POS

# LOCAL STORAGE
spatialArray = [[0 for x in range (0,10)] for x in range(0,21)]
freqArray= [[0 for x in range(0,21)] for x in range(0,21)]
spatialSize=len(indexToPosDict)+1
cooccurSize=len(indexToNameDict)+1

# WRITEFILE PATH
#cooccurFile=open("csv/cooccurrenceFreq.csv","w") 
#spatialFile=open("csv/spatialFreq.csv","w")
#TEST WRITEFILE PATH
cooccurFile=open("csv/coocTest.csv","w") 				
spatialFile=open("csv/spaTest.csv","w")			

# DATASET PATH
#path='/home/ian-djakman/Documents/data/voc_dataset/VOCdevkit/VOC2012/SegmentationClass'
path='img/' #local test path
fileQuant = glob.glob(os.path.join(path, '*.png'))
total=len(fileQuant)

meta=
metaConstructor.constructOccurrenceMetadata(path)
knowledgeConstructor.constructCoocurrenceFromMeta()
knowledgeConstructor.constructSpatial()



	


####################################################### PRINTING ####################################################################
	
# Print cooccurrence frequency info into system and file
print("")
print("")
print("Cooccurrence Frequency from your array :")
print("")
# column label
print("%11s"%"", end=" ")
cooccurFile.write(" ,")
for x in range (1,21):
	lbl = knowledgeConstructor.freqArray[0][x]
	print("%11s"%lbl, end=" ")
	cooccurFile.write("%s,"%lbl)	
print("")
cooccurFile.write("\n")
# row label and value
for x in range (1,21):
	lbl = freqArray[x][0]
	print("%11s"%lbl, end=" ")
	cooccurFile.write("%s,"%lbl)
	for y in range (1,21):
		val = freqArray[x][y]
		print(("%11d"%val),end=" ")
		cooccurFile.write("%d,"%val)
	print("")
	cooccurFile.write("\n")
cooccurFile.close()
print("")

# Print spatial frequency info into system
print("")
print("")
print("Spatial Frequency from your array :")
print("")
# column label
print("%14s"%"", end=" ") # for leftmost part, let it be blank for crossing label
spatialFile.write(" ,")
for x in range (1,10):
	lbl = spatialArray[0][x]
	print("%14s"%lbl, end=" ")
	spatialFile.write("%s,"%lbl)
print("")
spatialFile.write("\n")
# row label and value
for x in range (1,21):
	lbl = spatialArray[x][0]
	print("%14s"%lbl, end=" ")
	spatialFile.write("%s,"%lbl)
	for y in range (1,10):
		val = spatialArray[x][y]
		print(("%14d"%val),end=" ")
		spatialFile.write("%d,"%val)
	print("")
	spatialFile.write("\n")
spatialFile.close()
print("")
