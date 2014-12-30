#ifndef SVM_STRUCT_SS_LOSS_FUNCTION_H
#define SVM_STRUCT_SS_LOSS_FUNCTION_H

#include "api_types.h"

namespace svm_struct_ss {
namespace loss_function {

double zero_one_loss(const LABEL& y, const LABEL& ybar, 
                     const bool& as_sum_of_label_element_losses=false,
                     const bool& normalized=false) {
  debug_in_msg("zero_one_loss");
  debug_var("as_sum_of_label_element_losses",as_sum_of_label_element_losses);
  assert(y.size==ybar.size && "y.size!=ybar.size");

  double loss = 0.0; // optimistic init
  for (size_t i=0; i<y.size; ++i) {
    if (y.flatten_label[i]==svm_struct_ss::data_param::void_label) 
      continue;

    if (y.flatten_label[i] != ybar.flatten_label[i]) {
      if (as_sum_of_label_element_losses) {
        loss = loss + 1.0;
      }
      else {
        loss = 1.0;
        break;  
      }
    }
  }

  if (normalized) 
    loss = loss / y.size;

  debug_out_msg("zero_one_loss");
  return loss;
}

double hinge_loss() {
  // TODO
}

}
}

#endif