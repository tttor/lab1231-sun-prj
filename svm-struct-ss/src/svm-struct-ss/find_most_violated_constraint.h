#ifndef SVM_STRUCT_SS_FIND_MOST_VIOLATED_CONSTRAINT_H
#define SVM_STRUCT_SS_FIND_MOST_VIOLATED_CONSTRAINT_H

#include <shotton/shotton.h>
#include <shotton/edge_potential.h>

#include "data_param.h"

namespace svm_struct_ss {
namespace find_most_violated_constraint {

// Compute: argmax_{ybar} loss(y,ybar)+psi(x,ybar)
LABEL margin_rescaling(PATTERN x, LABEL y, const STRUCTMODEL& mrf_model) {
  namespace sun = lab1231_sun_prj;

  //
  sun::shotton::DataParam data_param;
  data_param["name"] = svm_struct_ss::data_param::dataset;
  data_param["n_label"] = svm_struct_ss::data_param::n_label;
  data_param["ori_img_dir"] = svm_struct_ss::data_param::ori_jpg_dir;
  data_param["unary_potential_dir"] = svm_struct_ss::data_param::unary_potential_dir;

  sun::shotton::EnergyParam energy_internal_param;
  sun::shotton::train(data_param, &energy_internal_param);

  //
  svm_struct_ss::util::MRFWeight energy_external_param(y.height,y.width);
  svm_struct_ss::util::get_weight(mrf_model, &energy_external_param);
  // debug_var("energy_external_param.unary_weight_MatrixXd().size()",energy_external_param.unary_weight_MatrixXd().size());
  // debug_var("energy_external_param.horizontal_pairwise_weight_MatrixXd().size()",energy_external_param.horizontal_pairwise_weight_MatrixXd().size());
  // debug_var("energy_external_param.vertical_pairwise_weight_MatrixXd().size()",energy_external_param.vertical_pairwise_weight_MatrixXd().size());

  bool used_as_loss_augmented_inference = true;
  svm_struct_ss::util::LabelMatrix label_mat;
  label_mat = sun::shotton::annotate(x.id, data_param, energy_internal_param, 
                                    energy_external_param.unary_weight_MatrixXd(),
                                    energy_external_param.horizontal_pairwise_weight_MatrixXd(),
                                    energy_external_param.vertical_pairwise_weight_MatrixXd(),
                                    used_as_loss_augmented_inference);
  
  //
  LABEL y_star;
  y_star = svm_struct_ss::util::get_LABEL(label_mat);

  return y_star;
} 

}
}
#endif