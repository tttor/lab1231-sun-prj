import os
import glob
import math
import voc_obj_intolbl
import voc_obj_lbltoin
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
indexToNameDict=voc_obj_intolbl.LABELS
nameToIndexDict=voc_obj_lbltoin.LABELS
	
def segment_image(image):
	img = img_as_float(image)
	return slic(img, n_segments=140, compactness=13, sigma=4, enforce_connectivity=True)

def show_images():
	files_to_read = glob.glob(os.path.join(training_image_list, '*.bmp'))
	total = len(files_to_read)
	for (num, imgfile) in enumerate(files_to_read):
		image = Image.open(imgfile)
		img = img_as_float(image)
		segmentation = segment_image(image) 
		
		fig, ax = plt.subplots(1, 2)
		fig.set_size_inches(8, 3, forward=True)
		fig.subplots_adjust(0.05, 0.05, 0.95, 0.95, 0.05, 0.05)
		
		ax[0].imshow(image)
		ax[0].set_title("Original")
		ax[1].imshow(mark_boundaries(img, segmentation))
		ax[1].set_title("SLIC")
		
		for a in ax:
			a.set_xticks(())
			a.set_yticks(())
		plt.show()

def main():
	global training_image_list
	training_image_list = '/home/ian-djakman/Desktop/gould_cow/tes'
	show_images()

if __name__ == '__main__':
    main()
