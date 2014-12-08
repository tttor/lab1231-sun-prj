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
	return imgfile[len(training_image_list):][:-4]
	
def segment_image(image):
	img = img_as_float(image)
	return slic(img, n_segments=140, compactness=13, sigma=4, enforce_connectivity=True)
	
def get_segment_in_given_class(gt_map, givenclass,segment, size):
	print ""

def get_centroid_and_weight(segments_for_given_class, segmentation, sgmnt, size):
	print ""

def get_norm_elements(centroid, size, x, y):
	print ""

def gould_relative_location_probability_map():
		
		probability_map = [[[[0 for x in range (0,200)] for x in range (0,200)] for x in range (0,23)] for x in range (0,23)]
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
			segment_list = [x for x in range (0, (len(numpy.unique(segmentation))))]

			image_info_class= [[[] for x in range (0, imageWidth)] for x in range (0, imageHeight)]
			image_info_seg= [[[] for x in range (0, imageWidth)] for x in range (0, imageHeight)]
			class_info_pixel = [[] for x in range (0,23)]
			segment_coor_h = [[] for x in range (0, len(segment_list))]
			segment_coor_w = [[] for x in range (0, len(segment_list))]
			
			#carikan centroid, weight, dan class dari segment tersebut
			for h in range (0, imageHeight):
				for w in range (0, imageWidth):
					pixclass = gt_map[h][w]
					pixseg   = segmentation[h][w]
					image_info_class[h][w].append(pixclass)
					image_info_seg[h][w].append(pixseg)
					segment_coor_h[pixseg].append(h)
					segment_coor_w[pixseg].append(w)
					class_info_pixel[pixclass].append([h,w])
			
			for each_segment in range (0, len(segment_list)):
				segment_to_process = segment_list[each_segment]
				segment_centroid = (int(round(sum(segment_coor_h[segment_to_process])/len(segment_coor_h[segment_to_process]))),int(round(sum(segment_coor_w[segment_to_process])/len(segment_coor_w[segment_to_process]))))
				segment_weight = len(segment_coor_h[segment_to_process])
				segment_class = image_info_class[segment_centroid[0]][segment_centroid[1]]
				
				for each_class in range (1,4):								# tak perlu periksa class -1 karena loop ini
					pixel_list_for_class = class_info_pixel[each_class]
					for pixel in range (0, len(pixel_list_for_class)):
						this_pix_h = pixel_list_for_class[pixel][0]		
						this_pix_w = pixel_list_for_class[pixel][1]
						hdiff = math.fabs(segment_centroid[0] - this_pix_h)
						wdiff = math.fabs(segment_centroid[1] - this_pix_w)
						norm_h = (hdiff / imageHeight) * 200
						norm_w = (wdiff / imageWidth) * 200
						offset =  math.hypot(hdiff, wdiff)
						norm_offset =  math.hypot(norm_h, norm_w)
						
						probability_map[segment_class[0]][each_class][int(norm_h)][int(norm_w)] = probability_map[segment_class[0]][each_class][int(norm_h)][int(norm_w)] + (segment_weight * norm_offset)
						
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
