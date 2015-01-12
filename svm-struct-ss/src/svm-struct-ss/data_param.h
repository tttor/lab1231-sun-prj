// Specify the properties of data

#ifndef SVM_STRUCT_SS_DATA_PARAM_H
#define SVM_STRUCT_SS_DATA_PARAM_H

#include <string>

namespace svm_struct_ss {
namespace data_param {

const std::string dataset = "voc2010";
const std::string n_label = "21";
const int void_label = 255;

const std::string unary_potential_dir = "/home/tor/xprmnt/philipp-unary/unary-scaled-0.25";
const std::string gt_csv_dir = "/home/tor/dataset/pascal/VOC2010/VOCdevkit/VOC2010/SegmentationClass-csv-scaled-0.25";
const std::string ori_jpg_dir = "/home/tor/dataset/pascal/VOC2010/VOCdevkit/VOC2010/JPEGImages-ClassSegmented-scaled-0.25";

}  
}

#endif