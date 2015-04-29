#!/usr/bin/python
import matplotlib.pyplot as plt
from skimage import io
import matplotlib.gridspec as gridspec

id_img = '2007_000032'
ori_dir = '/home/tor/dataset/pascal/VOC2010/VOCdevkit/VOC2010/JPEGImages-ClassSegmented'
gt_dir = '/home/tor/dataset/pascal/VOC2010/VOCdevkit/VOC2010/SegmentationClass'
ann_dir = '/home/tor/xprmnt/philipp-unary-mrf-voc2010/result/split_voc2010_philipp_Test_cls'
out_dir = '/home/tor/Dropbox/robotics/prj/011/wrt/tor/kc-benchmarker/s2is-usings2is/fig'

path_ori = ori_dir +'/'+ id_img + '.jpg'
path_gt = gt_dir +'/'+ id_img + '.png'
path_ann = ann_dir +'/'+ id_img + '.png'

image_file_ori = io.imread(path_ori)
image_file_gt = io.imread(path_gt)
image_file_ann = io.imread(path_ann)

f = plt.figure()

gs = gridspec.GridSpec(1, 3, width_ratios=[1,1,1],height_ratios=[1,1,1])

ax1 = plt.subplot(gs[0])
ax2 = plt.subplot(gs[1])
ax3 = plt.subplot(gs[2])

ax1.imshow(image_file_ori,aspect='auto')
ax2.imshow(image_file_gt,aspect='auto')
ax3.imshow(image_file_ann,aspect='auto')

ax1.axis('off')
ax2.axis('off')
ax3.axis('off')

out_filepath = out_dir+'/'+id_img+'_ori_gt_ann.png'
f.savefig(out_filepath,bbox_inches = 'tight')
