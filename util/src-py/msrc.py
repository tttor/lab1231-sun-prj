# Based on http://drwn.anu.edu.au/drwnProjMultiSeg.html
color_class_map = {
	(0, 0, 0) :255, #case on accurate GT from Phillipp
	(64,64,64) :255, #case on accurate GT from Phillipp
	(128, 0, 0) : 0, 
	(0, 128, 0) : 1, 
	(128, 128, 0) : 2, 
	(0, 0, 128) : 3, 
	(0, 128, 128) : 4, 
	(128, 128, 128) : 5, 
	(192, 0, 0) : 6, 
	(64, 128, 0) : 7, 
	(192, 128, 0) : 8, 
	(64, 0, 128) : 9, 
	(192, 0, 128) : 10,
	(64, 128, 128) : 11,
	(192, 128, 128) : 12,
	(0, 64, 0) : 13,
	(128, 64, 0) : 14,
	(0, 192, 0) : 15,
	(128, 64, 128) : 16,
	(0, 192, 128) : 17,
	(128, 192, 128) : 18,
	(64, 64, 0) : 19,
	(192, 64, 0) : 20,
	(64, 0, 0) : 21,
	(128, 0, 128) : 22
}

class_name_map = {
	0 : "building",
	1 : "grass",
	2 : "tree",
	3 : "cow",
	4 : "sheep",
	5 : "sky",
	6 : "airplane",
	7 : "water",
	8 : "face",
	9 : "car",
	10: "bicycle",
	11: "flower",
	12: "sign",
	13: "bird",
	14: "book",
	15: "chair",
	16: "road",
	17: "cat",
	18: "dog",
	19: "body",
	20: "boat",
	21: "mountain",
	22: "horse",
	255: "void"
}

class_id2name_map = class_name_map

class_name2id_map = {v: k for k, v in class_name_map.items()}

ori_img_format = '.bmp'
ignored_class_name_list = ['void','horse','mountain']
