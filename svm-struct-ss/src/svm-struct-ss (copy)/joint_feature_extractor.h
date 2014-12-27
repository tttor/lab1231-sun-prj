#ifndef SVM_STRUCT_SS_JOINT_FEATURE_EXTRACTOR_H
#define SVM_STRUCT_SS_JOINT_FEATURE_EXTRACTOR_H

#include <svm_light/svm_common.h>
#include <vector>

#include "util.h"
#include "example_param.h"
#include "io.h"

namespace svm_struct_ss {
namespace joint_feature_extractor {

void psi(PATTERN x, LABEL y, SVECTOR* fvec) {
  debug_in_msg("joint_feature_extractor::psi");
  using namespace std;
  // TODO @tttor: how to handle missing features: make them zero-valued?

  // get unary Philipp prob image
  debug_msg("computing unary fea:...");
  ProbImage unary_prob_img;
  const string prob_img_filepath = string(svm_struct_ss::data_param::unary_potential_dir+"/"+x.id+".unary");
  debug_var("prob_img_filepath",prob_img_filepath);
  unary_prob_img.load(prob_img_filepath.c_str());
  debug_msg("got unary_prob_img");

  // get unary feature matrix whose size is constant
  Eigen::Matrix<FVAL, Eigen::Dynamic, Eigen::Dynamic> unary_fea_mat;
  unary_fea_mat = Eigen::Matrix<FVAL, Eigen::Dynamic, Eigen::Dynamic>::Zero(SVM_STRUCT_SS_EXAMPLE_PARAM_MAX_IMAGE_HEIGHT,SVM_STRUCT_SS_EXAMPLE_PARAM_MAX_IMAGE_WIDTH);

  for (long r=0; r<unary_fea_mat.rows(); ++r) {
    for (long c=0; c<unary_fea_mat.cols(); ++c) {
      long flat_idx = svm_struct_ss::util::flat_idx( r,c,unary_prob_img.width() );

      // the top left corner of this feature matrix always coincides with the top left corner of max feature matrix
      // the uncovered area is where the feature values are missing, for which we use the initial values
      // TODO @tttor should we apply log to the unary prob?
      if (flat_idx < y.size)
        unary_fea_mat(r,c) = unary_prob_img( c,r,y.flatten_label[flat_idx] );
    }
  }

  // row-wise flattening
  unary_fea_mat.transposeInPlace();
  Eigen::Matrix<FVAL, Eigen::Dynamic, 1> unary_fea_vec( Eigen::Map< Eigen::Matrix<FVAL, Eigen::Dynamic, 1> >(unary_fea_mat.data(),unary_fea_mat.cols()*unary_fea_mat.rows()) );
  debug_msg("got unary fea");
  // for (long i=0; i<10; ++i) {
  //   debug_var("unary_fea_vec(i)",unary_fea_vec(i));
  // }

  // // // get pairwise features
  // // Eigen::Matrix<FVAL, Eigen::Dynamic, Eigen::Dynamic> horizontal_pairwise_fea_mat;
  // // Eigen::Matrix<FVAL, Eigen::Dynamic, Eigen::Dynamic> vertical_pairwise_fea_mat;

  // fill in the SVECTOR
  // Note: WORD's wnum begin at 1, therefore: wnum = word_idx + 1
  FNUM word_idx = 0;
  for (FNUM i=0; i<unary_fea_vec.size(); ++i) {
    fvec->words[word_idx].wnum = word_idx + 1;
    fvec->words[word_idx].weight = unary_fea_vec(i);
    ++word_idx;
  }

  // for (FNUM i=0; i<horizontal_pairwise_fea_vec; ++i) {
  //   fvec->words[word_idx].wnum = word_idx + 1;
  //   fvec->words[word_idx].weight = unary_fea_vec[i];
  //   ++word_idx;
  // }

  // for (FNUM i=0; i<vertical_pairwise_fea_vec; ++i) {
  //   fvec->words[word_idx].wnum = word_idx + 1;
  //   fvec->words[word_idx].weight = unary_fea_vec[i];
  //   ++word_idx;
  // }

  // for (long i=0; i<10; ++i) {
  //   debug_var("fvec->words[i].wnum",fvec->words[i].wnum);
  //   debug_var("fvec->words[i].weight",fvec->words[i].weight);
  // }
  debug_out_msg("joint_feature_extractor::psi");
}

}
}

#endif