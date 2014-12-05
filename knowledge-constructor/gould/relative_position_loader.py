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

def main():
	outfile = "/home/ian-djakman/Desktop/gould_relative_position.npy"
	probability_map = np.load(outfile)
	tes = (probability_map[int(nameToIndexDict['aeroplane'])] [int(nameToIndexDict['aeroplane'])])
	plt.imshow(tes)
	plt.show(tes)

if __name__ == '__main__':
    main()
