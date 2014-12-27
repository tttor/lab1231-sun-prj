#ifndef SVM_STRUCT_SS_UTIL_H
#define SVM_STRUCT_SS_UTIL_H

#include <svm_light/svm_common.h>

#include "api_types.h"
#include "data_param.h"
#include "io.h"
#include "example_param.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cassert>

#include <Eigen/Dense>
#include "../../../external/prob-image/probimage.h"

namespace svm_struct_ss {
namespace util {

typedef 
Eigen::Matrix<int,Eigen::Dynamic,Eigen::Dynamic, Eigen::RowMajor> 
LabelMatrix;

typedef 
Eigen::Matrix<FVAL, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> 
FeatureMatrix;

struct MRFWeight {
  Eigen::MatrixXd unary_weight;
  Eigen::MatrixXd horizontal_pairwise_weight;
  Eigen::MatrixXd vertical_pairwise_weight;
};

MRFWeight get_weight(const STRUCTMODEL& mrf_model, const long& label_width, const long& label_height) {
  // Eigen::MatrixXd unary_weight();
  // unary_weight = 

  // Eigen unary_weight;
  // unary_weight()

  MRFWeight mrf_w;
  // mrf_w.unary_weight = 

  return mrf_w;
}

// row-wise idx flattening
long flat_idx_rowcol(long row, long col, long n_col) {
  return  col + (row * n_col);
}

LABEL get_LABEL(const LabelMatrix& label_mat) {
  LABEL label;

  label.width = label_mat.cols();
  label.height = label_mat.rows();
  label.size = label.height*label.width;
  label.max_size = sizeof(label.flatten_label)/sizeof(label.flatten_label[0]);
  for (long j=0; j<label.size; ++j) label.flatten_label[j] = label_mat.data()[j];// row-wise flattening

  return label;
}

SAMPLE get_set_of_examples(const std::string& list_filepath) {
  using namespace std;
  //
  std::vector<std::string> list;
  list = svm_struct_ss::io::read_list(list_filepath);

  //
  const long n_example = list.size();
  debug_var("n_example=",n_example);

  EXAMPLE* examples;
  examples = (EXAMPLE *)my_malloc(sizeof(EXAMPLE)*n_example);

  for(long i=0; i<n_example; ++i) {
    cout << "Reading example= " << i+1 << " of " << n_example << endl;

    //
    PATTERN pattern;
    strcpy(pattern.id, list.at(i).c_str());

    //
    LABEL label;

    string gt_csv_filepath = string(svm_struct_ss::data_param::gt_csv_dir+"/"+list.at(i)+".csv");
    LabelMatrix label_mat = svm_struct_ss::io::read_csv<LabelMatrix>(gt_csv_filepath);
    label = get_LABEL(label_mat);

    //
    examples[i].x = pattern;
    examples[i].y = label;
  }

  //
  SAMPLE set_of_examples;
  set_of_examples.n = n_example;
  set_of_examples.examples = examples;

  return set_of_examples;
}

long get_n_feature() {
  const long n_unary_fea = SVM_STRUCT_SS_EXAMPLE_PARAM_MAX_N_PIXEL;
  const long n_pairwise_fea = SVM_STRUCT_SS_EXAMPLE_PARAM_MAX_N_PAIRWISE_POTENTIAL;

  // return n_unary_fea+n_pairwise_fea;
  return n_unary_fea;
} 

}// namespace util
}// namespace svm_struct_ss

#endif