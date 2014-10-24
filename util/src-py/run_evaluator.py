#!/usr/bin/python

import sys
from evaluator import Evaluator
from msrc import color_class_map

def main():
	if len(sys.argv) != 6:
		assert False, "INSUFFICIENT ARGUMENTS!"

	data_param = {"dataset_name": sys.argv[1], "n_label": int(sys.argv[2]), "gt_img_dir": sys.argv[3], "test_img_list_filepath": sys.argv[4], "result_dir": sys.argv[5], "mapping_label": color_class_map, "dir_output": sys.argv[5]}

	#TODO @tttor: re-implement Evaluator
	# eval = Evaluator(data_param)
	# eval.evaluate()
	
	# filenames = [line.strip() for line in open(data_param["test_img_list_filepath"])]
	# ann_filepaths = [data_param["result_dir"]+filename[:len(filename)-4]+".ann" for filename in filenames]

	# evaluator = Evaluator(ann_filepaths, data_param["gt_img_dir"], data_param)
	# evaluator.evaluate_all()

if __name__ == "__main__":
	main();