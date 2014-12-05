import os
import glob
import math
import voc_obj_intolbl
import voc_obj_lbltoin
import voc_spa_intopos
import datetime
from PIL import Image
from io import StringIO, BytesIO

import cv2
import numpy as np
from skimage.segmentation import slic
from skimage.segmentation import mark_boundaries
import matplotlib.pyplot as plt
from skimage.util import img_as_float
from scipy import ndimage

#dictionary
indexToNameDict=voc_obj_intolbl.LABELS
nameToIndexDict=voc_obj_lbltoin.LABELS
indexToPosDict=voc_spa_intopos.POS

def get_filename(imgfile):
	return imgfile[len(training_image_list):][:12]
	
def segment_image(image):
	img = img_as_float(image)
	return slic(img, n_segments=200, compactness=25, sigma=2)
	
def get_segment_in_given_class(ann_img, givenclass,segment, pixel, size):
	segment_list_for_class = []
	(imageWidth, imageHeight) = size
	for x in range (0,imageWidth):
		for y in range (0, imageHeight):
			if pixel[x,y] != 0 and pixel[x,y] != 255:
				if indexToNameDict[str(pixel[x,y])] == givenclass:
					segment_list_for_class.append(segment[y][x])
	return list(set(segment_list_for_class))

def get_centroid_and_weight(segments_for_given_class, ann_img, segmentation, sgmnt, size):
	(imageWidth, imageHeight) = size
	segment_to_process = segments_for_given_class[sgmnt]
	pixel_coordinate_x = []
	pixel_coordinate_y = []
	for i in range (0, imageHeight):
		for j in range (0, imageWidth):
			if segmentation[i][j] == segment_to_process:
				pixel_coordinate_x.append(i)
				pixel_coordinate_y.append(j)
	centroid_val = ((sum(pixel_coordinate_x) /len(pixel_coordinate_x)),(sum(pixel_coordinate_y)/len(pixel_coordinate_y)))
	return centroid_val, len(pixel_coordinate_x)

def get_norm_elements(centroid, ann_img, x, y):
	imageSize = ann_img.size
	(imageWidth, imageHeight) = imageSize
	cen_h = centroid[0]
	cen_w = centroid[1]
	height_diff = math.fabs(cen_h - y)
	norm_height = (height_diff / imageHeight ) * 200
	width_diff =  math.fabs(cen_w - x)
	norm_width = (width_diff / imageWidth ) * 200
	offset =  math.hypot(height_diff, width_diff)
	norm_offset =  math.hypot(norm_height, norm_width)
	return norm_offset, norm_height, norm_width

def gould_relative_location_probability_map():
		
		probability_map = [[[[0 for x in range (0,200)] for x in range (0,200)] for x in range (0,21)] for x in range (0,21)]
		files_to_read = glob.glob(os.path.join(training_image_list, '*.jpg'))
		total = len(files_to_read)
		
		for (num, imgfile) in enumerate(files_to_read):
			print("Processing " + str(num+1) +"/" + str(total) + " image for Gould relative map") # Progress Feedback
			filename = get_filename(imgfile)
			image = Image.open(imgfile)
			annpath = annotation_path + filename + '.png'
			ann_image = Image.open(annpath)
			imageSize = ann_image.size
			(imageWidth, imageHeight) = imageSize
			pixel = ann_image.load()
			segmentation = segment_image(image) 
			
			for each_class in range (0,len(indexToNameDict)):
				given_class = indexToNameDict[str(each_class+1)]
				segment_list_for_given_class = get_segment_in_given_class(ann_image, given_class, segmentation, pixel, imageSize)
				for segments in range (0, len(segment_list_for_given_class)):
					centroid, weight = get_centroid_and_weight(segment_list_for_given_class, ann_image, segmentation, segments, imageSize)
														
					offset = 0
					for x in range(0,imageWidth):
						for y in range(0,imageHeight):
							if pixel[x,y] != 0 and pixel[x,y] != 255:
								pixel_class = pixel[x,y]
								norm_offset, norm_height, norm_width = get_norm_elements(centroid, ann_image, x, y)
								norm_weight = weight * (40000 / float(imageWidth*imageHeight))
								probability_map[(each_class+1)][pixel_class][int(norm_height)][int(norm_width)] = weight * norm_offset
						

		tes = (probability_map[int(nameToIndexDict['aeroplane'])] [int(nameToIndexDict['aeroplane'])])
		plt.imshow(tes)
		plt.savefig('tes.jpg')
		outfile = "/home/ian-djakman/Desktop/gould_relative_position"
		np.save(outfile,probability_map)
		plt.show(tes)
		
def main():
	dataset_name = "voc"
	global training_image_list
	global annotation_path
	annotation_path =  '/home/ian-djakman/Documents/data/input/voc_dataset_2012/SegmentationClass'
	#class_list = get_class_of(dataset_name)
	#training_image_list = get_training_image_list(dataset_name)
	training_image_list = '/home/ian-djakman/online/Github/lab1231-sun-prj/knowledge-constructor/test'
	global probability_map
	gould_relative_location_probability_map()

if __name__ == '__main__':
    main()
