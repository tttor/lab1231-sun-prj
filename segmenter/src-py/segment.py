#!/usr/bin/python

import sys
import os
import matplotlib.pyplot as plt
import numpy as np

from skimage.segmentation import felzenszwalb, slic, quickshift
from skimage import color
from skimage.segmentation import mark_boundaries
from skimage.util import img_as_float
from skimage import io

def doSegment(param, img):
  if param[0] == 'slic':
    segments_res = slic(img, n_segments=int(param[1]), compactness=int(param[2]), sigma=int(param[3]), convert2lab=True)
  elif param[0] == 'pff':
    segments_res = felzenszwalb(img, scale=int(param[1]), sigma=float(param[2]), min_size=int(param[3]))
  elif param[0] == 'quick':
    segments_res = quickshift(img, kernel_size=int(param[1]), max_dist=int(param[2]), ratio=float(param[3]), convert2lab=True)
  return segments_res

def ensure_path(path):
  # directory = path
  directory = path[:path.rfind('/')]
  if not os.path.exists(directory):
    os.makedirs(directory)

def segmentsToFile(path_output, segments):
  map_segment = {}
  for i in range(np.max(segments)+1):
    map_segment.setdefault(i, [])
  
  for row in range(len(segments)):
    for col in range(len(segments[row])):
      map_segment[segments[row][col]].append(row*len(segments[row]) + col)
  str_buf = ''
  for x in xrange(len(map_segment)):    
    str_buf += str(map_segment[x]).strip('[]').replace(" ", "") + '\n'
  ensure_path(path_output)
  with open(path_output, 'w') as the_file:
    the_file.write(str_buf)

def main():
  # param 1 : path-list-gambar, param 2 : path-list-param, param 3 : dir-image, param4 : dir-output
  if len(sys.argv) != 5:
    assert False, "INSUFFICIENT ARGUMENTS!"
    
  filenames = [line.strip() for line in open(sys.argv[1])]
  not_include = ['1_12_s.bmp', '1_13_s.bmp', '1_14_s.bmp', '11_4_s.bmp', '11_5_s.bmp', '1_16_s.bmp', '11_8_s.bmp', '12_31_s.bmp', '12_33_s.bmp', '12_3_s.bmp', '1_26_s.bmp', '1_27_s.bmp', '12_7_s.bmp', '12_8_s.bmp', '14_13_s.bmp', '14_14_s.bmp', '14_15_s.bmp', '14_16_s.bmp', '14_17_s.bmp', '14_20_s.bmp', '14_5_s.bmp', '15_13_s.bmp', '15_19_s.bmp', '15_5_s.bmp', '15_6_s.bmp', '15_7_s.bmp', '16_16_s.bmp', '16_23_s.bmp', '16_27_s.bmp', '16_8_s.bmp', '17_15_s.bmp', '17_17_s.bmp', '17_19_s.bmp', '17_23_s.bmp', '17_26_s.bmp', '17_30_s.bmp', '17_5_s.bmp', '17_8_s.bmp', '19_15_s.bmp', '19_30_s.bmp', '19_7_s.bmp', '1_9_s.bmp', '20_6_s.bmp', '2_13_s.bmp', '2_14_s.bmp', '2_17_s.bmp', '2_18_s.bmp', '2_22_s.bmp', '2_25_s.bmp', '2_26_s.bmp', '2_28_s.bmp', '2_7_s.bmp', '2_8_s.bmp', '3_12_s.bmp', '3_16_s.bmp', '3_22_s.bmp', '3_26_s.bmp', '3_29_s.bmp', '3_5_s.bmp', '4_16_s.bmp', '4_20_s.bmp', '4_23_s.bmp', '4_26_s.bmp', '4_28_s.bmp', '4_30_s.bmp', '4_3_s.bmp', '5_11_s.bmp', '5_13_s.bmp', '5_14_s.bmp', '5_23_s.bmp', '5_27_s.bmp', '5_28_s.bmp', '5_2_s.bmp', '5_3_s.bmp', '6_15_s.bmp', '6_16_s.bmp', '6_22_s.bmp', '7_10_s.bmp', '7_12_s.bmp', '7_14_s.bmp', '7_15_s.bmp', '7_21_s.bmp', '7_22_s.bmp', '7_25_s.bmp', '8_10_s.bmp', '8_18_s.bmp', '8_19_s.bmp', '9_13_s.bmp', '9_17_s.bmp', '9_18_s.bmp', '9_19_s.bmp', '9_20_s.bmp', '9_24_s.bmp', '9_26_s.bmp', '9_5_s.bmp']
  filenames = [ filename for filename in filenames if filename not in not_include]

  count = 1;
  for filename in filenames:
    print 'segment image : ' + str(count) + '/' + str(len(filenames))
    count+=1
    file_path = (sys.argv[3]).strip() + '/' + filename
    for line in open(sys.argv[2]):      
      image = img_as_float(io.imread(file_path))
      param = line.strip().split('-')
      segments_res = doSegment(param, img=image)
      path_output=sys.argv[4] + filename[:len(filename)-4] + '/' + filename[:len(filename)-4]+'-'+'-'.join(param)+'.sup'
      seg_img = mark_boundaries(image, segments_res)
      io.imsave(sys.argv[4] + '/image/' + filename[:len(filename)-4]+'-'+'-'.join(param)+'.bmp', seg_img)
      segmentsToFile(path_output, segments_res)    

if __name__ == "__main__":
  main();
