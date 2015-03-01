import sys
import cPickle
import datetime as dt
from lxml import etree

def construct(img_ids, scepe_prop_ann_dir):
    ann_filepaths = [scepe_prop_ann_dir+'/'+img_id+'.xml' for img_id in img_ids]

    scene_prop = {}
    for i, img_id in enumerate(img_ids):
        print 'Processing',i+1,'of',len(img_ids),'id=',img_id

        ann_filepath = scepe_prop_ann_dir+'/'+img_id+'.xml'
        root = etree.parse(ann_filepath).getroot()
        for place_sub in root.findall('place'):
            place = place_sub.get('name').lower()
            objs = [obj_sub.get('name').lower() for obj_sub in place_sub.findall('object')]
            
            if place in scene_prop.keys():
                obj_count_dict = scene_prop[place]
                for obj in objs:
                    if obj in obj_count_dict.keys():
                        obj_count_dict[obj] = obj_count_dict[obj] + 1
                    else:
                        obj_count_dict[obj] = 1
            else:
                obj_count_dict = dict.fromkeys(objs, 1)
                scene_prop[place] = obj_count_dict

    return scene_prop

def write_xml(scene_prop, xml_filepath):
    '''
    Write an xml file of this knowledge.
    '''
    root = etree.Element("scene_property_knowledge")
    root.set("dataset","pascal_voc_2010")
    root.set('timestamp', str(dt.datetime.now()))
    
    for key, obj_count_dict in scene_prop.items():
        sub = etree.SubElement(root, 'place')
        sub.set('name', key)

        subsub = etree.SubElement(sub, 'property')
        total = sum([i for i in obj_count_dict.values()])
        subsub.set('n_obj',str(len(obj_count_dict.keys())))
        subsub.set('n_obj_occurrence',str(total))

        for subkey, subval in obj_count_dict.items():
            subsubsub = etree.SubElement(subsub, subkey)
            subsubsub.set('freq', str(subval))
            subsubsub.set('norm_freq',str( float(subval)/total ))
        
    writer = etree.ElementTree(root)
    writer.write(xml_filepath, pretty_print=True)

def write(scene_prop, filepath):
    with open(filepath, 'wb') as fid:
        cPickle.dump(scene_prop, fid) 

def read(xml_filepath, normalized=True):
    assert normalized,'normalized != True'
    scene_prop = {}

    tree = etree.parse(xml_filepath)
    root = tree.getroot()
    
    for sub in root:
        obj_freq_dict = {}
        for sub_2 in sub:
            for sub_3 in sub_2:
                obj_freq_dict[sub_3.tag] = float(sub_3.attrib['norm_freq'])            
        scene_prop[sub.attrib['name']] = obj_freq_dict

    return scene_prop

def main(argv):
    assert len(argv)==4, 'INSUFFICENT NUMBER OF ARGUMENTS'
    img_id_list_filepath = argv[1]
    scepe_prop_ann_dir = argv[2]
    knowledge_output_dir = argv[3]

    with open(img_id_list_filepath) as f:
        img_ids = f.readlines()
    img_ids = [x.strip('\n') for x in img_ids]

    #
    scene_prop = construct(img_ids, scepe_prop_ann_dir)
    write(scene_prop, knowledge_output_dir+'/scene_property.pickle')
    write_xml(scene_prop, knowledge_output_dir+'/scene_property.xml')

if __name__ == '__main__':
    main(sys.argv)
