#!/usr/bin/python

def predict(ann_filepath, knowledge_dir, ml_model_filepath):
    # Extract features
    cooccurrence_knowledge = kc.read(knowledge_dir+'/cooccurrence-knowledge/cooccurrence.xml')
    cooccurrence_fea = []

    sceneprop_knowledge = ks.read(knowledge_dir+'/scene-property-knowledge/scene_property.xml')
    sceneprop_fea = []

    relloc_knowledge = kr.read(knowledge_dir+'/relative-location-knowledge/relloc-pickle')
    relloc_fea = []

    ann = {}
    ann['ann'] = np.loadtxt(ann_filepath, delimiter=',')
    ann['filename'] = sample_id_list[i]
    ann['ori_img_dir'] = ori_img_dir
    ann['ori_img_ext'] = '.jpg'

    cooccurrence_fea = xtt.extract_cooccurrence_fea(ann,cooccurrence_knowledge)
    sceneprop_fea = xtt.extract_sceneprop_fea(ann,sceneprop_knowledge)
    relloc_fea = xtt.extract_relloc_fea(ann,relloc_knowledge)

    # Load and Do predict
    estimator = 
    y_pred = estimator.predict(X_te)

def main():
	pass

if __name__ == '__main__':
	main()