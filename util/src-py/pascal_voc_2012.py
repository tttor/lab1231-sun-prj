'''
http://pascallin.ecs.soton.ac.uk/challenges/VOC/voc2012/htmldoc/index.html

For each test image pixel, predict the class of the object containing that pixel or 'background' 
if the pixel does not belong to one of the twenty specified classes. 

The output from your system should be an indexed image with each pixel index 
indicating the number of the inferred class (1-20) or zero, indicating background.
'''
class_name_map = {
    0 : 'background',
    1 : 'aeroplane',
    2 : 'bicycle',
    3 : 'bird',
    4 : 'boat',
    5 : 'bottle',
    6 : 'bus',
    7 : 'car',
    8 : 'cat',
    9 : 'chair',
    10 : 'cow',
    11 : 'diningtable',
    12 : 'dog',
    13 : 'horse',
    14 : 'motorbike',
    15 : 'person',
    16 : 'pottedplant',
    17 : 'sheep',
    18 : 'sofa',
    19 : 'train',
    20 : 'tvmonitor',
    255: 'void'
}
id2name_map = class_name_map
class_id2name_map = class_name_map

ori_img_format = '.jpg'
ignored_class_name_list = ['void','background']

def translate(numeric_classes):
    string_classes = [id2name_map[i] for i in numeric_classes]
    return string_classes

def get_id(given_name):
    for num, name in id2name_map.iteritems():
        if name == given_name:
            return num