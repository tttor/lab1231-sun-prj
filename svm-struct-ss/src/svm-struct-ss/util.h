#ifndef SVM_STRUCT_SS_UTIL_H
#define SVM_STRUCT_SS_UTIL_H

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

// row-wise idx flattening
long flat_idx(long row, long col, long n_col) {
  return  col + (row * n_col);
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
    label.max_size = sizeof(label.flatten_label)/sizeof(label.flatten_label[0]);

    string gt_csv_filepath = string(svm_struct_ss::data_param::gt_csv_dir+"/"+list.at(i)+".csv");
    Eigen::MatrixXi label_mat = svm_struct_ss::io::read_csv<Eigen::MatrixXi>(gt_csv_filepath);

    // row-wise flattening
    label_mat.transposeInPlace();
    Eigen::VectorXi label_vec(Eigen::Map<Eigen::VectorXi>(label_mat.data(), label_mat.cols()*label_mat.rows()));
    for (long j=0; j<label_vec.size(); ++j) label.flatten_label[j] = label_vec(j);

    label.size = label_vec.size();
    debug_var("label_vec.size()",label_vec.size());

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