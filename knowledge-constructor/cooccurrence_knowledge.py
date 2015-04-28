import sys
sys.path.append('../util/src-py')

import cPickle
import numpy as np
import numpy as np
import matplotlib.pyplot as plt
import datetime as dt
from lxml import etree

import pascal_voc_2012 as voc

'''
@construct()
Based on pixel-wise annotations, this construct a cooccurrence knowledge.

The knowledge can be represented by a n-by-n matrix C, 
where n is the number of the prespecified object classes in a dataset.
The element C[i,i] indicates the number of occurrence of i in the dataset.
The element C[i,j] indicates the number of cooccurrence of i and j in the 
dataset and is equal to C[j,i].

Notice that 
> we do not care whether an object class is from two or more
separate objects, meaning that there is no information about the cooccurrence
of an object with itself.
> the data stucture for the cooccurrence knowledge is a dictionary.
'''
def construct(ann_ids, obj_names, ann_dir):
    cooccurrence = dict.fromkeys(obj_names, None)
    for key in cooccurrence.iterkeys():
        cooccurrence[key] = dict.fromkeys(obj_names, 0)

    for i,ann_id in enumerate(ann_ids):
        print 'Processing',i+1,'of',len(ann_ids),'ann_id=',ann_id

        ann_filepath = ann_dir+'/'+ann_id+'.csv'
        ann = np.genfromtxt(ann_filepath, delimiter=',')

        obj_ids = list(set( ann.flatten().tolist() ))
        objs = voc.translate(obj_ids)
        objs = [i for i in objs if i not in voc.ignored_class_name_list]

        for i in objs:
            cooccurrence[i][i] = cooccurrence[i][i] + 1
            for j in objs:            
                if i is not j:
                    cooccurrence[i][j] = cooccurrence[i][j] + 1

    return cooccurrence
    
def write_matrix(cooccurrence, matrix_filepath):
    '''
    Draw a matrix like a confusion-matrix to better intrepet the knowledge.
    '''
    mat = np.array([[cooccurrence[o1][o2] for o2 in sorted(cooccurrence[o1])] for o1 in sorted(cooccurrence)])

    # plotting
    fig = plt.figure()
    ax = fig.add_subplot(111)
    ax.set_aspect(1)
    
    res = ax.imshow(np.array(mat), cmap=plt.cm.jet, interpolation='nearest')

    height,width = mat.shape
    for x in xrange(width):
        for y in xrange(height):
            ax.annotate(str(mat[x][y]), xy=(y, x), size = 7, horizontalalignment='center', verticalalignment='center')

    alphabet = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'
    plt.xticks(range(width), alphabet[:width])
    plt.yticks(range(height), alphabet[:height])

    plt.savefig(matrix_filepath)
    plt.close()

def write_xml(cooccurrence, xml_filepath):
    '''
    Write an xml file of this knowledge.
    '''
    root = etree.Element("cooccurrence_knowledge")
    root.set("dataset","pascal_voc_2010")
    root.set('timestamp', str(dt.datetime.now()))
    
    for key, inner_dict in cooccurrence.items():
        sub = etree.SubElement(root, 'object')
        sub.set('name', key)
        sub.set('id', str(voc.get_id(key)))

        subsub = etree.SubElement(sub, 'occurrence')
        subsub.set('freq', str(cooccurrence[key][key]))

        subsub_2 = etree.SubElement(sub, 'co-occurrence')
        total = 0
        for subkey, subval in inner_dict.items():
            if subkey != key:
                total = total + subval
        subsub_2.set('total',str(total))

        for subkey, subval in inner_dict.items():
            if subkey != key:                
                subsubsub = etree.SubElement(subsub_2, subkey)
                subsubsub.set('freq', str(subval))
                subsubsub.set('norm_freq',str( float(subval)/total ))
        
    writer = etree.ElementTree(root)
    writer.write(xml_filepath, pretty_print=True)

def write(cooccurrence, filepath):
    with open(filepath, 'wb') as fid:
        cPickle.dump(cooccurrence, fid) 

def read(xml_filepath, normalized=True):
    '''
    Read the xml containing the cooccurrence knowledge.
    Read only the normalized frequency.
    '''
    assert normalized,'normalized != True'
    cooccurrence = {}

    tree = etree.parse(xml_filepath)
    root = tree.getroot()
    
    for sub in root:
        local_knowledge = {}
        for sub_2 in sub:
            if sub_2.tag=='occurrence':
                continue
            for sub_3 in sub_2:
                local_knowledge[sub_3.tag] = float(sub_3.attrib['norm_freq'])
        cooccurrence[sub.attrib['name']] = local_knowledge

    return cooccurrence

# def normalize(raw_cooccurrence):
#     '''
#     Normalize the frequency of cooccurrence of an object i with another object j
#     to be in the range of [0,1].
#     '''
#     # cooccurrence = dict.fromkeys(raw_cooccurrence.keys(), dict.fromkeys(obj_names, 0.0))# this does _not_ work, TODO why?
#     cooccurrence = dict.fromkeys(obj_names, None)
#     for key in cooccurrence.iterkeys():
#         cooccurrence[key] = dict.fromkeys(obj_names, 0.0)

#     for key,raw_inner_dict in raw_cooccurrence.iteritems():
#         # of objects other than object 'key'
#         total = sum([val2 for key2,val2 in raw_inner_dict.iteritems if key2 is not key])

#         for key2,val2 in cooccurrence[key].iteritems():
#             norm_freq = 1.0
#             if key2 is not key:
#                 norm_freq = float(raw_cooccurrence[key][key2]) / total
#             cooccurrence[key][key2] = norm_freq;

#     return cooccurrence

def main(argv):
    assert len(argv)==4, 'INSUFFICENT NUMBER OF ARGUMENTS'
    ann_list_filepath = argv[1]
    csv_ann_dir = argv[2]
    knowledge_dir = argv[3]
    
    obj_names = voc.translate( [key for key in voc.id2name_map if key!=0 and key!=255] )

    with open(ann_list_filepath) as f:
        ann_ids = f.readlines()
    ann_ids = [x.strip('\n') for x in ann_ids]

    cooccurrence = {}
    cooccurrence = construct(ann_ids,obj_names,csv_ann_dir)
    write_xml(cooccurrence, knowledge_dir+'/cooccurrence.xml')
    write_matrix(cooccurrence, knowledge_dir+'/cooccurrence.png')
    write(cooccurrence, knowledge_dir+'/cooccurrence.pickle')

if __name__ == '__main__':
    main(sys.argv)
