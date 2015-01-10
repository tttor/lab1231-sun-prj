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

// The weight matrix is row-wise mat, following the feature matrix
typedef
Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> 
WeightMatrix;

class MRFWeight {
 public:
  MRFWeight(size_t height, size_t width) {
    unary_weight = WeightMatrix::Ones(height,width);
    horizontal_pairwise_weight = WeightMatrix::Ones(height,width-1);
    vertical_pairwise_weight = WeightMatrix::Ones(height-1,width);
  }

  ~MRFWeight() {};

  WeightMatrix unary_weight;
  WeightMatrix horizontal_pairwise_weight;
  WeightMatrix vertical_pairwise_weight;

  // These return default column-wise double matrices
  inline Eigen::MatrixXd unary_weight_MatrixXd() {return unary_weight;}
  inline Eigen::MatrixXd horizontal_pairwise_weight_MatrixXd() {return horizontal_pairwise_weight;}
  inline Eigen::MatrixXd vertical_pairwise_weight_MatrixXd() {return vertical_pairwise_weight;}
};

// row-wise idx flattening
inline size_t flat_idx_rowcol(size_t row, size_t col, size_t n_col) {
  return  col + (row * n_col);
}

inline size_t flat_idx_xy(const size_t& x, const size_t& y, const size_t& n_col) {
  return x + (n_col * y);
}

inline size_t example_max_height() {
  return SVM_STRUCT_SS_EXAMPLE_PARAM_MAX_IMAGE_HEIGHT;
}

inline size_t example_max_width() {
  return SVM_STRUCT_SS_EXAMPLE_PARAM_MAX_IMAGE_WIDTH;
}

inline size_t get_n_unary_feature() {
  return SVM_STRUCT_SS_EXAMPLE_PARAM_MAX_N_PIXEL;
}

inline size_t get_n_horizontal_pairwise_feature() {
  return (example_max_width()-1) * example_max_height();
}

inline size_t get_n_vertical_pairwise_feature() {
  return example_max_width() * (example_max_height()-1);
}

inline size_t get_n_pairwise_feature() {
  assert(SVM_STRUCT_SS_EXAMPLE_PARAM_N_NEIGHBOUR==4 && "fatal: only N4 neigborhood system is supported");
  return get_n_horizontal_pairwise_feature() + get_n_vertical_pairwise_feature();
}

inline size_t get_n_feature() {
  return get_n_unary_feature() + get_n_pairwise_feature();
} 

// Pull out the local label-depend-sized feature matrix _out_ of the global constant-size feature matrix.
// Their top-left corners coincide.
// This also applies to the weight matrix since the weight and the feature matrices are in one-on-one correspondence
// The weights in sm.w correspond to the features defined by psi() and range from index 1 to index sm->sizePsi. 
// Following the feature array, the weight array is set with the following order: 
// (1) unary, (2) horizontal pairwise, (3) vertical pairwise, subsequently
// TODO should we use sm.w _or_ sm.svm_model.lin_weights ( weights for linear case using folding)? Are they are always equal?
void get_weight(const STRUCTMODEL& mrf_model, MRFWeight* mrf_w) {
  debug_in_msg("get_weight");

  // The weights in sm.w range from index 1 to index sm->sizePsi.
  size_t w_idx = 1;

  // unary
  WeightMatrix unary_weight(example_max_height(), example_max_width());
  for (size_t i=0; i<unary_weight.size(); ++i, ++w_idx) unary_weight.data()[i] = mrf_model.w[w_idx];
  mrf_w->unary_weight = unary_weight.topLeftCorner(mrf_w->unary_weight.rows(),mrf_w->unary_weight.cols());
  
  // pairwise
  WeightMatrix horizontal_pairwise_weight(example_max_height(), (example_max_width()-1));
  for (size_t i=0; i<horizontal_pairwise_weight.size(); ++i, ++w_idx) horizontal_pairwise_weight.data()[i] = mrf_model.w[w_idx];
  mrf_w->horizontal_pairwise_weight = horizontal_pairwise_weight.topLeftCorner(mrf_w->horizontal_pairwise_weight.rows(), 
                                                                               mrf_w->horizontal_pairwise_weight.cols());

  WeightMatrix vertical_pairwise_weight(example_max_height()-1, example_max_width());
  for (size_t i=0; i<vertical_pairwise_weight.size(); ++i, ++w_idx) vertical_pairwise_weight.data()[i] = mrf_model.w[w_idx];
  mrf_w->vertical_pairwise_weight = vertical_pairwise_weight.topLeftCorner(mrf_w->vertical_pairwise_weight.rows(), 
                                                                           mrf_w->vertical_pairwise_weight.cols());

  //
  assert(w_idx==mrf_model.sizePsi+1 && "FATAL:w_idx!=mrf_model.sizePsi+1");
  debug_out_msg("get_weight");
}

LABEL get_LABEL(const LabelMatrix& label_mat) {
  LABEL label;

  label.width = label_mat.cols();
  label.height = label_mat.rows();
  label.size = label.height*label.width;
  label.max_size = sizeof(label.flatten_label)/sizeof(label.flatten_label[0]);
  for (size_t j=0; j<label.size; ++j) label.flatten_label[j] = label_mat.data()[j];// row-wise flattening as LabelMatrix is row-major

  return label;
}

size_t get_n_examples(const std::string& list_filepath) {
  std::vector<std::string> list;
  list = io::read_list(list_filepath);

  return list.size();
}

void set_examples(const std::vector<std::string>& list, EXAMPLE *examples) {
  using namespace std;
  size_t n_example = list.size();
  for(size_t i=0; i<n_example; ++i) {
    cout << "Reading example= " << i+1 << " of " << n_example << endl;

    //
    PATTERN pattern;
    strcpy(pattern.id, list.at(i).c_str());

    //
    LABEL label;

    string gt_csv_filepath = string(data_param::gt_csv_dir+"/"+list.at(i)+".csv");
    LabelMatrix label_mat = io::read_csv<LabelMatrix>(gt_csv_filepath);
    label = get_LABEL(label_mat);

    //
    examples[i].x = pattern;
    examples[i].y = label;
  }
}

}// namespace util
}// namespace svm_struct_ss

#endif