import sys
import os
import matplotlib.pyplot as plt
import numpy as np
import csv

from skimage.segmentation import felzenszwalb, slic, quickshift
from skimage import color
from skimage.segmentation import mark_boundaries
from skimage.util import img_as_float
from skimage import io
from operator import itemgetter

img_dir = '/home/jogie/sun3/dataset/msrc/unmix/Images/ppm/'

def ensure_path(path):
  directory = path[:path.rfind('/')]
  if not os.path.exists(directory):
    os.makedirs(directory)

def get_index_bestdoc(val, N=25):
    list_index = []
    ind = range(0, len(val))
    zip_ind_val = zip(ind, val)
    zip_ind_val = sorted(zip_ind_val,key=itemgetter(1), reverse=True)
    prob = [val[1] for val in zip_ind_val[0:N]]
    for i in range(N):
        list_index.append(zip_ind_val[i][0])
    return list_index

def csv2Array(csv_path):
    with open(csv_path) as f:
        reader = csv.reader(f)
        arr = list(reader)
    for row in range(len(arr)):
        for col in range(len(arr[row])):
            arr[row][col] = int(arr[row][col])
    return np.asarray(arr)

def createFigure4(list_file, list_param, corpus_meta, prob_topic_doc, segment_dir, n_topic, output_dir):
    for file_item in list_file:
        print file_item
        for topic in range(n_topic):
            print topic
            fig = plt.figure()
            index=1
            for param in list_param:
                segment_in_file = [item_corpus for item_corpus in corpus_meta if str(file_item+'-'+param) == item_corpus[1].split('-')[0]]
                segments_res = csv2Array(segment_dir+'/'+file_item+'/'+file_item+'-'+param+'.sup')
                img = img_as_float(io.imread(img_dir+file_item+'.ppm'))
                output = np.zeros( (len(img), len(img[0])) )
                for segment in segment_in_file:
                    output[segments_res == int(segment[2])] = prob_topic_doc[int(segment[0])][topic] 
                output = mark_boundaries(output, segments_res)
                ax1 = fig.add_subplot(5,10, index, axisbg='grey')
                ax1.set_xticks(()), ax1.set_yticks(())
                ax1.imshow(output)
                index += 1 
            ensure_path(output_dir+'/'+file_item+'/')
            plt.savefig(output_dir+'/'+file_item+'/topic-'+str(topic)+'-'+file_item)
            plt.clf()
            plt.close()
                # ensure_path(output_dir+'/'+file_item+'/'+str(topic)+'/')
                # io.imsave(output_dir+'/'+file_item+'/'+str(topic)+'/'+file_item+'-'+str(topic) + '-'+param+'.png', output)  

def createFigure5(prob_topic_doc, corpus_meta, segment_dir, n_topic, output_dir):
    all_result = {}
    for i in range(n_topic):
        all_result[i] = []
        arr = [doc[i] for doc in prob_topic_doc]
        list_index_bestdoc = get_index_bestdoc(arr, N=25)
        for ind in list_index_bestdoc:
            all_result[i].append( (corpus_meta[ind][1], int(corpus_meta[ind][2])) )
    for i in range(n_topic):
        fig = plt.figure()
        index = 1
        print i
        for param, ind in all_result[i]:
            img_name = param.split('-')[0]
            segments_res = csv2Array(segment_dir+'/'+img_name+'/'+param)
            img = img_as_float(io.imread(img_dir+img_name+'.ppm'))  
            img[segments_res != ind] = (0.0,0.0,0.9)
            ax1 = fig.add_subplot(5,5, index, axisbg='grey')
            ax1.set_xticks(()), ax1.set_yticks(())
            ax1.imshow(img)
            index += 1    
        plt.savefig(output_dir+'/topic-'+str(i))
        plt.clf()
        plt.close()

def plot_top_N_segment_in_topic(list_file, segment_dir, corpus_meta, prob_topic_doc, n_topic, N, output_dir):
    for name_file in list_file:
        print name_file
        for topic in range(n_topic):
            print topic
            fig = plt.figure()
            index_plt = 1
            ind_segment = [int(item_corpus[0]) for item_corpus in corpus_meta if name_file == item_corpus[1].split('-')[0]]
            val_segment = [prob_topic_doc[ind][topic] for ind in ind_segment]
            ind_of_ind_segment = get_index_bestdoc(val_segment, 10)
            for index in ind_of_ind_segment:
                segments_res = csv2Array(segment_dir+'/'+name_file+'/'+ corpus_meta[ind_segment[index]][1])
                no_segment = int(corpus_meta[ind_segment[index]][2])
                output = img_as_float(io.imread(img_dir+name_file+'.ppm'))
                output[segments_res != no_segment] = (0.0,0.0,0.9)
                ax1 = fig.add_subplot(4,3, index_plt, axisbg='grey')
                ax1.set_xticks(()), ax1.set_yticks(())
                ax1.set_title(prob_topic_doc[ind_segment[index]][topic])
                ax1.imshow(output)
                index_plt += 1
            ensure_path(output_dir+'/'+name_file+'/')
            plt.savefig(output_dir+'/'+name_file+'/'+str(topic))
            plt.clf()
            plt.close()            

def main():
    #to do -> call with args
    output_dir = '/home/jogie/sorter_exp/exp_result/msrc/141209-500K/'
    segment_dir = '/home/jogie/sun4/exp/overlap-segment/superpixel-3'
    sup_param_dir = '/home/jogie/sun4/exp/overlap-segment/meta/segment-param.list'
    list_path = '/home/jogie/sun4/exp/overlap-segment/meta/test591.list'
    theta_final_path = '/home/jogie/sorter_exp/lda-model/training.20141208.194731/model-final.theta'
    corpus_meta_path = '/home/jogie/sorter_exp/lda-model/training.20141208.194731/corpus.20141208.165442.meta'
    
    prob_topic_doc = [ line.strip().split(' ') for line in open(theta_final_path)]
    
    for row in range(len(prob_topic_doc)):
        for col in range(len(prob_topic_doc[row])):
            prob_topic_doc[row][col] = float(prob_topic_doc[row][col])

    corpus_meta = [ line.strip().split(' ') for line in open(corpus_meta_path)]
    
    createFigure5(prob_topic_doc, corpus_meta, segment_dir, 25 ,output_dir+'figure5/')
    
    list_file = [ line.strip('\n') for line in open(list_path)]
    list_param = [ line.strip('\n') for line in open(sup_param_dir)]
    
    createFigure4(list_file, list_param, corpus_meta, prob_topic_doc, segment_dir, 25, output_dir+'figure4/')

    plot_top_N_segment_in_topic(list_file_new, segment_dir, corpus_meta, prob_topic_doc, 25, 10, output_dir+'figureNew')

if __name__ == "__main__":
    main();