#ifndef SVM_STRUCT_SS_LOSS_FUNCTION_H
#define SVM_STRUCT_SS_LOSS_FUNCTION_H

#include "api_types.h"

namespace svm_struct_ss {
namespace loss_function {

double zero_one_loss(const LABEL& y, const LABEL& ybar) {
  assert(y.size==ybar.size && "y.size!=ybar.size");

  double loss = 0.0; // optimistic init
  for (size_t i=0; i<y.size; ++i) {
    if (y.flatten_label[i]==svm_struct_ss::data_param::void_label) 
      continue;

    if (y.flatten_label[i] != ybar.flatten_label[i]) {
      loss = 1.0;
      break;  
    } 
  }
  return loss;
}

double hamming_loss(const LABEL& y, const LABEL& ybar) {
  assert(y.size==ybar.size && "y.size!=ybar.size");

  double loss = 0.0; // optimistic init
  for (size_t i=0; i<y.size; ++i) {
    if (y.flatten_label[i]==svm_struct_ss::data_param::void_label) 
      continue;

    if (y.flatten_label[i] != ybar.flatten_label[i])
      loss = loss + 1.0;
  }
  loss = loss / y.size;

  return loss;
}

double hinge_loss() {
  // TODO
}

}
}

#endif