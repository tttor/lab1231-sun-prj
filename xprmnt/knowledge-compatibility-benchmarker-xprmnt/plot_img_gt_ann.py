#!/usr/bin/python
import yaml
import matplotlib.pyplot as plt
import matplotlib.gridspec as gridspec
from skimage import io

ori_dir = '/home/tor/dataset/pascal/VOC2010/VOCdevkit/VOC2010/JPEGImages-ClassSegmented'
gt_dir = '/home/tor/dataset/pascal/VOC2010/VOCdevkit/VOC2010/SegmentationClass'
ann_dir = '/home/tor/xprmnt/philipp-unary-mrf-voc2010/result/split_voc2010_philipp_Test_cls'
out_dir = '/home/tor/Dropbox/robotics/prj/011/wrt/tor/kc-benchmarker/s2is-usings2is/fig/regression/qual-result'
data_dict_filepath = '/home/tor/xprmnt/knowledge-compatibility-benchmarker/regression-result/philippunarymrf-voc2010-GradientBoostingRegressor-20150501.164425/best_regressor_wrt_r2.delta'

def main():
    data_dict = None
    with open(data_dict_filepath) as f:  
        data_dict = yaml.load(f)

    for imgid,data in data_dict.iteritems():
        y_true = data[0]
        y_pred = data[1]
        delta = data[2]

        if is_ok_4(delta):
            print 'plotting delta=', delta, 'of', imgid
            plot_ori_gt_ann(imgid)

            with open(out_dir+'/'+imgid+'.data','w') as f:
                f.write(str(y_true)+'\n')
                f.write(str(y_pred)+'\n')
                f.write(str(delta)+'\n')


def is_ok_1(delta):
    if delta < 0.001:
        return True
    else:
        return False

def is_ok_2(delta):
    if (delta >= 0.001) and (delta < 0.010):
        return True
    else:
        return False

def is_ok_3(delta):
    if (delta >= 0.010) and (delta < 0.100):
        return True
    else:
        return False

def is_ok_4(delta):
    if (delta >= 0.100) and (delta < 1.000):
        return True
    else:
        return False

def is_ok_5(delta):
    if (delta >= 1.000):
        return True
    else:
        return False

def plot_ori_gt_ann(imgid):
    path_ori = ori_dir +'/'+ imgid + '.jpg'
    path_gt = gt_dir +'/'+ imgid + '.png'
    path_ann = ann_dir +'/'+ imgid + '.png'

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

    out_filepath = out_dir+'/'+imgid+'_ori_gt_ann.png'
    f.savefig(out_filepath,bbox_inches = 'tight')
    plt.close(f)

if __name__ == '__main__':
    main()