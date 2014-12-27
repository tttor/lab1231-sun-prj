#ifndef SVM_STRUCT_SS_FIND_MOST_VIOLATED_CONSTRAINT_H
#define SVM_STRUCT_SS_FIND_MOST_VIOLATED_CONSTRAINT_H

using namespace svm_struct_ss {
using namespace find_most_violated_constraint {

// Compute: argmax_{ybar} loss(y,ybar)+psi(x,ybar)
LABEL margin_rescaling(PATTERN x, LABEL y, STRUCTMODEL* model, STRUCT_LEARN_PARM* hyperparam) {
  namespace sun = lab1231-sun-prj;

  //
  Eigen::MatrixXi label_mat;
  label_mat = sun::shotton::annotate(img_filename, data_param, energy_param);


  //
  LABEL y_star;
  return y_star;
} 

}
}
#endif