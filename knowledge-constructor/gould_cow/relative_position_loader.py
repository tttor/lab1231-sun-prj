import os
import glob
import math
import msrc_obj_intolbl
import msrc_obj_lbltoin
import datetime
from PIL import Image
from io import StringIO, BytesIO

import cv2
import numpy as np
from skimage.segmentation import slic
from skimage.segmentation import mark_boundaries
import matplotlib.pyplot as plt
import matplotlib.cm as cm
from skimage.util import img_as_float
from scipy import ndimage
import json

#dictionary
indexToNameDict=msrc_obj_intolbl.LABELS
nameToIndexDict=msrc_obj_lbltoin.LABELS

def main():
	with open('/home/ian-djakman/Desktop/gould_cow/out/gould_relative_position_map.json', 'r') as fp:
		kv = json.load(fp)
	probability_map = kv['prob_map']
	tes = (probability_map[int(nameToIndexDict['grass'])] [int(nameToIndexDict['cow'])])
	plt.imshow(tes, cmap = cm.Greys_r)
	plt.savefig('/home/ian-djakman/Desktop/gould_cow/out/img/tes.jpg')
	plt.show(tes)

if __name__ == '__main__':
    main()
