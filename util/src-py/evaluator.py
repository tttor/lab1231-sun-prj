import cv2
import numpy

class Evaluator:
	def __init__(self, param):
		self.param = param
		self.list_acc_class = []
		self.conf_mat = self.init_conf_mat()

	def init_conf_mat(self):
		conf_mat = []
		n_label = self.param["n_label"]	+ 1
		for i in range(n_label):
			conf_mat.append([0] * n_label)
		return conf_mat

	def get_label_from_bgr(self, px_color):
		return self.param["mapping_label"][px_color]

	def make_list_from_ann_file(self, file_path):
		input_text = open(file_path,'r')
		output = []
		for row in input_text:
			element_of_row = row.split(',')
			element_of_row[-1].replace('\n','')
			for element in element_of_row:
				output.append(int(element))
		input_text.close()
		return output

	def make_list_from_img_file(self, img_path):
		output = []
		print 'img_path=', img_path
		img = cv2.imread(img_path)
		r,c,d = img.shape
		for rr in range(r):
			for cc in range(c):				
				px = (img[rr,cc,2], img[rr,cc,1], img[rr,cc,0])
				label_of_px = self.get_label_from_bgr(px)
				output.append(label_of_px)
		return output

	def evaluate(self, list_ann_label, list_gt_label):
		conf_mat = self.init_conf_mat()
		for ann_label, gt_label in zip (list_ann_label, list_gt_label):
			self.conf_mat[gt_label][ann_label] += 1
			conf_mat[gt_label][ann_label] += 1
		list_acc_class = self.calc_acc_each_class(conf_mat)
		return conf_mat, list_acc_class

	def evaluate_all(self):
		self.conf_mat = self.init_conf_mat()
		for x in xrange(len(self.param["ann_filepaths"])): 
			list_ann_label = self.make_list_from_ann_file(self.param["ann_filepaths"][x])
			gt_file_name = self.param["gt_img_dir"] + self.param["ann_filepaths"][x].split('/')[-1].replace('ann','bmp')
			list_gt_label = self.make_list_from_img_file(gt_file_name)
			res_conf_mat, res_list_acc = self.evaluate(list_ann_label, list_gt_label)
			average_acc = self.get_avg_acc(res_list_acc)
			file_output = self.param["dir_output"] + self.param["ann_filepaths"][x].split('/')[-1].replace('ann','out')			
			self.write_to_file(file_output, res_list_acc, res_conf_mat, average_acc)
			print "image " + str(x) + " done"
		file_output = self.param["dir_output"] + 'global.out'
		list_acc_class_global = self.calc_acc_each_class(self.conf_mat)
		average_acc = self.get_avg_acc(res_list_acc)
		self.write_to_file(file_output, list_acc_class_global, self.conf_mat, average_acc)
			

	def write_to_file(self, file_output, list_acc_class, conf_mat, average_acc):
		with open(file_output, 'w') as the_file:
				str_buf = 'average acc: ' + str(average_acc) + '\n'
				str_buf += 'accuracy class: ' + str(list_acc_class).strip('[]') + '\n'
				str_buf += 'confusion matrix:\n'				
				for row in conf_mat:
					str_buf += str(row).strip('[]') + '\n'
				the_file.write(str_buf)

	def calc_acc_each_class(self, conf_mat):
		list_res = []
		n_label = self.param["n_label"] + 1
		for j in range(n_label):
			all_gtj = sum (conf_mat[j])
			all_predj = sum([item[j] for item in conf_mat]) 
			gtj_predj = conf_mat[j][j]	
			devided = (all_gtj + all_predj - gtj_predj)
			acc = 100 *  (float(gtj_predj) / devided ) if all_gtj > 0 else -1
			list_res.append(acc)
		return list_res

	def get_avg_acc(self, list_acc):
		li_acc = [item for item in list_acc if item >= 0]
		return numpy.mean(li_acc)