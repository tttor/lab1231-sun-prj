import voc_obj_intolbl
import voc_obj_lbltoin

indexToNameDict=voc_obj_intolbl.LABELS
nameToIndexDict=voc_obj_lbltoin.LABELS

#class Cooccurrence:
def constructCoocurrenceFromMeta() :
	
	cooccurFile=open("csv/coocTest.csv","w") 	
	cooccurSize=len(indexToNameDict)+1

	#preparing label name in the array
	for label in range (1,cooccurSize):
		freqArray[0][label]=indexToNameDict[str(label)]
		freqArray[label][0]=indexToNameDict[str(label)]


	metaToRead=open("metadata/cooccurrenceMeta.txt","rU")
	print("")
	for line in metaToRead.readlines():
		setList = line.split(",")
		setSize= len(setList)
	
		# Frequency List, including mutual appearance
		for x in range(0, setSize):
			for y in range (0, setSize):
				freqArray[int(setList[x])][int(setList[y])] = freqArray[int(setList[x])][int(setList[y])] + 1
	metaToRead.close()
	
def constructSpatial() :
	#preparing label name in the array
	for label in range (1,spatialSize):
		spatialArray[0][label]=indexToPosDict[str(label)]
	for label in range (1,cooccurSize):
		spatialArray[label][0]=indexToNameDict[str(label)]

	for (num, infile) in enumerate(fileQuant):
		datasetImage = Image.open(infile)
		pixel        = datasetImage.load()
		imageSize    = datasetImage.size
		(imageWidth, imageHeight)  = imageSize
	
		leftLimit = math.floor(imageWidth/2)
		rightLimit = imageWidth
		topLimit = math.floor(imageHeight/2)
		bottomLimit = imageHeight
		centerWStart = math.floor(imageWidth/4)
		centerWLimit = math.floor(imageWidth*3/4)
		centerHStart = math.floor(imageHeight/4)
		centerHLimit = math.floor(imageHeight*3/4)
	
		print("Processing " + str(num+1) +"/" + str(total) + " files for Spatial Relation", end="\r") # Proccessing Progress Feedback
	
	
		###################### TOP PROCESSING ############################
	
		#Read Pixel Value
		pixelLabelListPart = [pixel[i,j] for i in range(0,rightLimit) for j in range (0,topLimit) if pixel[i, j] != 0 and pixel[i, j] != 255]
		setListPart = list(set(pixelLabelListPart))
		setSize= len(setListPart)
	
		# Spatial Frequency entry, every part do n iteration
		for x in range(0, setSize):
			spatialArray[setListPart[x]][1] = spatialArray[setListPart[x]][1] + 1
		
		###################### BOTTOM PROCESSING #########################
	
		#Read Pixel Value
		pixelLabelListPart = [pixel[i,j] for i in range(0,rightLimit) for j in range (topLimit,bottomLimit) if pixel[i, j] != 0 and pixel[i, j] != 255]
		setListPart = list(set(pixelLabelListPart))
		setSize= len(setListPart)
	
		# Spatial Frequency entry, every part do n iteration
		for x in range(0, setSize):
			spatialArray[setListPart[x]][2] = spatialArray[setListPart[x]][2] + 1
		
		###################### LEFT PROCESSING ############################
	
		#Read Pixel Value
		pixelLabelListPart = [pixel[i,j] for i in range(0,leftLimit) for j in range (0,bottomLimit) if pixel[i, j] != 0 and pixel[i, j] != 255]
		setListPart = list(set(pixelLabelListPart))
		setSize= len(setListPart)
	
		# Spatial Frequency entry, every part do n iteration
		for x in range(0, setSize):
			spatialArray[setListPart[x]][3] = spatialArray[setListPart[x]][3] + 1
		
		###################### RIGHT PROCESSING ###########################
	
		#Read Pixel Value
		pixelLabelListPart = [pixel[i,j] for i in range(leftLimit,rightLimit) for j in range (0,bottomLimit) if pixel[i, j] != 0 and pixel[i, j] != 255]
		setListPart = list(set(pixelLabelListPart))
		setSize= len(setListPart)
	
		# Spatial Frequency entry, every part do n iteration
		for x in range(0, setSize):
			spatialArray[setListPart[x]][4] = spatialArray[setListPart[x]][4] + 1
		
		################# TOP LEFT PROCESSING #########################
	
		#Read Pixel Value
		pixelLabelListPart = [pixel[i,j] for i in range(0,leftLimit) for j in range (0,topLimit) if pixel[i, j] != 0 and pixel[i, j] != 255]
		setListPart = list(set(pixelLabelListPart))
		setSize= len(setListPart)
	
		# Spatial Frequency entry, every part do n iteration
		for x in range(0, setSize):
			spatialArray[setListPart[x]][5] = spatialArray[setListPart[x]][5] + 1
		
		################# TOP RIGHT PROCESSING #########################
	
		#Read Pixel Value
		pixelLabelListPart = [pixel[i,j] for i in range(leftLimit,rightLimit) for j in range (0,topLimit) if pixel[i, j] != 0 and pixel[i, j] != 255]
		setListPart = list(set(pixelLabelListPart))
		setSize= len(setListPart)
	
		# Spatial Frequency entry, every part do n iteration
		for x in range(0, setSize):
			spatialArray[setListPart[x]][6] = spatialArray[setListPart[x]][6] + 1
		
		################# BOTTOM LEFT PROCESSING #########################
	
		#Read Pixel Value
		pixelLabelListPart = [pixel[i,j] for i in range(0,leftLimit) for j in range (topLimit,bottomLimit) if pixel[i, j] != 0 and pixel[i, j] != 255]
		setListPart = list(set(pixelLabelListPart))
		setSize= len(setListPart)
	
		# Spatial Frequency entry, every part do n iteration
		for x in range(0, setSize):
			spatialArray[setListPart[x]][7] = spatialArray[setListPart[x]][7] + 1
		
		################# BOTTOM RIGHT PROCESSING #########################
	
		#Read Pixel Value
		pixelLabelListPart = [pixel[i,j] for i in range(leftLimit,rightLimit) for j in range (topLimit,bottomLimit) if pixel[i, j] != 0 and pixel[i, j] != 255]
		setListPart = list(set(pixelLabelListPart))
		setSize= len(setListPart)
	
		# Spatial Frequency entry, every part do n iteration
		for x in range(0, setSize):
			spatialArray[setListPart[x]][8] = spatialArray[setListPart[x]][8] + 1
		
		################# CENTER PROCESSING #########################
	
		#Read Pixel Value
		pixelLabelListPart = [pixel[i,j] for i in range(centerWStart,centerWLimit) for j in range (centerHStart,centerHLimit) if pixel[i, j] != 0 and pixel[i, j] != 255]
		setListPart = list(set(pixelLabelListPart))
		setSize= len(setListPart)
	
		# Spatial Frequency entry, every part do n iteration
		for x in range(0, setSize):
			spatialArray[setListPart[x]][9] = spatialArray[setListPart[x]][9] + 1
