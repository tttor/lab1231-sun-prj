# custom py file import
from knowledgeConstructor import KnowledgeConstructor

# directory for training data and xml
datasetVOC="/home/ian-djakman/Documents/data/input/voc_dataset_2012/SegmentationClass"
xmlVOC="/home/ian-djakman/Documents/data/input/voc_dataset_2012/ckpstar_modified_annotation"
				
#Work to be done by the program is listed here
vocKC = KnowledgeConstructor("VocOutput")
vocKC.set_training_data_dir(datasetVOC)
vocKC.set_xml_data_dir(xmlVOC)
#vocKC.cooccurence_knowledge()
#vocKC.cooccurence_knowledge_from_xml()
#vocKC.spatial_knowledge(33.33 , 33.33 , 33.33)
#vocKC.scene_properties_knowledge ()
vocKC.relative_knowledge(20,60)

