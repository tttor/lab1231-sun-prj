#!/usr/bin/python
'''
Goal:
Given a set of images, make a gibbslda-compatible data format, see http://gibbslda.sourceforge.net/

Format:
[M]
[document(segment)1]
[document(segment)2]
...
[document(segment)M]

'''
from os import listdir
from os.path import isfile, join

import sys
sys.path.append('/home/tor/sun/ws/lab1231-sun-prj/segment-sorter/common')

import segment_sorter_util as util

def get_contained_region(regions, segment):
    '''
    For now: a simple check if the int of region center is in the segment
    '''
    contained_regions = [region for region in regions if (int(region['loc'][1]),int(region['loc'][0])) in segment]

    return contained_regions

def write_gibbslda_data(docs, filepath):
    fo = open(filepath, "wb")
    
    fo.write( str(len(docs))+'\n' )
    for doc in docs:
        doc_str = ''.join(i+' ' for i in doc)
        doc_str = doc_str + '\n'
        fo.write(doc_str)

    fo.close()

def main():
    list_filepath = '/media/tor/46DAF35ADAF344A9/tor/robotics/prj/011/xprmnt/segment-sorter/meta/msrc-21.sift.list'
    with open(list_filepath) as f:
        content = f.readlines()
    content = [i.strip('\n') for i in content]

    segment_dir = '/home/tor/sun4/xprmnt/segment-sorter/segment/msrc-21/superpixel-3'
    clustered_region_dir = '/home/tor/sun4/xprmnt/segment-sorter/region-clustered/msrc-21'
    docs = []

    for i, img_id in enumerate(content):
        print('Processing: %s (%i/%i)' % (img_id,i+1,len(content)))
        region_filepath = clustered_region_dir + '/' + img_id + '.sift.cls'
        regions = util.get_region_from_regfile(region_filepath, clustered=True)

        local_segment_dir = segment_dir + '/' + img_id
        segment_filepaths = [ local_segment_dir+'/'+f for f in listdir(local_segment_dir) if isfile(join(local_segment_dir,f)) ]

        for segment_filepath in segment_filepaths:# as for one image, we may do multiple segmentations
            segments = util.get_segment_from_supfile(segment_filepath)

            for segment in segments:
                contained_regions = get_contained_region(regions, segment)

                if len(contained_regions) != 0:
                    doc = ['word'+str(region['label']) for region in contained_regions]
                    docs.append(doc)

    print('n_document(=n_segment)= %i' % (len(docs)))
    if len(docs) != 0:
        training_data_filepath = '/home/tor/sun4/xprmnt/segment-sorter/lda-data/hot/msrc21.dat'
        write_gibbslda_data(docs, training_data_filepath)

if __name__ == '__main__':
    main()