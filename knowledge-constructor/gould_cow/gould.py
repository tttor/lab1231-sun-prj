import os
import glob
import math
import msrc_obj_intolbl
import msrc_obj_lbltoin
import datetime
from PIL import Image
from io import StringIO, BytesIO

import cv2
from skimage.segmentation import slic
from skimage.segmentation import mark_boundaries
import matplotlib.pyplot as plt
from skimage.util import img_as_float
from scipy import ndimage
import matplotlib.cm as cm
import json
import csv
import numpy

#dictionary
indexToNameDict=msrc_obj_intolbl.LABELS
nameToIndexDict=msrc_obj_lbltoin.LABELS

def get_filename(imgfile):
	return imgfile[len(training_image_list):][:6]
	
def segment_image(image):
	img = img_as_float(image)
	return slic(img, n_segments=140, compactness=13, sigma=4, enforce_connectivity=True)
	
def get_segment_in_given_class(gt_map, givenclass,segment, size):
	segment_list_for_class = []
	(imageWidth, imageHeight) = size
	for x in range (0,imageWidth):
		for y in range (0, imageHeight):
			if gt_map[y][x] != -1:
				if indexToNameDict[str(gt_map[y][x])] == givenclass:
					segment_list_for_class.append(segment[y][x])
	return list(set(segment_list_for_class))

def get_centroid_and_weight(segments_for_given_class, segmentation, sgmnt, size):
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

def get_norm_elements(centroid, size, x, y):
	(imageWidth, imageHeight) = size
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
		files_to_read = glob.glob(os.path.join(training_image_list, '*.bmp'))
		total = len(files_to_read)
		
		for (num, imgfile) in enumerate(files_to_read):
			print("Processing " + str(num+1) +"/" + str(total) + " image for Gould relative map") # Progress Feedback
			filename = get_filename(imgfile)
			image = Image.open(imgfile)
			annpath = annotation_path + filename + '.csv'
			gt_csv = csv.reader(open(annpath,'r'),delimiter=',')
			gt_list = list(gt_csv)
			gt_map = numpy.array(gt_list).astype('int')
			
			imageSize = image.size
			(imageWidth, imageHeight) = imageSize
			segmentation = segment_image(image) 
			
			for each_class in range (1,4):
				given_class = indexToNameDict[str(each_class)]
				segment_list_for_given_class = get_segment_in_given_class(gt_map, given_class, segmentation, imageSize)
				for segments in range (0, len(segment_list_for_given_class)):
					centroid, weight = get_centroid_and_weight(segment_list_for_given_class, segmentation, segments, imageSize)
														
					offset = 0
					for x in range(0,imageWidth):
						for y in range(0,imageHeight):
							if gt_map[y][x] != -1:
								pixel_class = gt_map[y][x]
								norm_offset, norm_height, norm_width = get_norm_elements(centroid, imageSize, x, y)
								norm_weight = weight * (40000 / float(imageWidth*imageHeight))
								probability_map[(each_class)][pixel_class][int(norm_height)][int(norm_width)] = probability_map[(each_class)][pixel_class][int(norm_height)][int(norm_width)] + (weight * norm_offset)
						
		keyval_for_json = {'prob_map' : probability_map}
		outfile = '/home/ian-djakman/Desktop/gould_cow/out/gould_relative_position_map.json'
		with open(outfile, 'w') as f:
			json.dump(keyval_for_json, f)

def main():
	dataset_name = "msrc"
	global training_image_list
	global annotation_path
	annotation_path =  '/home/ian-djakman/Desktop/gould_cow/gt_csv'
	training_image_list = '/home/ian-djakman/Desktop/gould_cow/tes'
	global probability_map
	gould_relative_location_probability_map()

if __name__ == '__main__':
    main()
