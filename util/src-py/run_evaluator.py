#!/usr/bin/python

import sys
from evaluator import Evaluator
from msrc import color_class_map

def main():
	if len(sys.argv) != 6:
		assert False, "INSUFFICIENT ARGUMENTS!"

	filenames = [line.strip() for line in open(sys.argv[4])]
	ann_filepaths = [sys.argv[5] + '/' + filename[:len(filename)-4]+".ann" for filename in filenames]
	# print(ann_filepaths)
	
	data_param = {"dataset_name": sys.argv[1], "ann_filepaths": ann_filepaths,"n_label": int(sys.argv[2]), "gt_img_dir": sys.argv[3], "test_img_list_filepath": sys.argv[4], "result_dir": sys.argv[5], "mapping_label": color_class_map, "dir_output": sys.argv[5]}

	eval = Evaluator(data_param)
	eval.evaluate_all()

if __name__ == "__main__":
	main();