#ifndef SVM_STRUCT_SS_JOINT_FEATURE_EXTRACTOR_H
#define SVM_STRUCT_SS_JOINT_FEATURE_EXTRACTOR_H

#include <svm_light/svm_common.h>
#include <vector>

#include "util.h"
#include "example_param.h"
#include "io.h"

namespace svm_struct_ss {
namespace joint_feature_extractor {

void psi(const PATTERN& x, const LABEL& y,const long& n_word, SVECTOR* fvec) {
  debug_in_msg("joint_feature_extractor::psi");
  using namespace std;

  // get unary Philipp prob image
  debug_msg("computing unary fea:...");
  ProbImage unary_prob_img;
  const string prob_img_filepath = string(svm_struct_ss::data_param::unary_potential_dir+"/"+x.id+".unary");
  // debug_var("prob_img_filepath",prob_img_filepath);
  unary_prob_img.load(prob_img_filepath.c_str());
  debug_msg("got unary_prob_img");

  // get unary joint feature
  svm_struct_ss::util::FeatureMatrix local_unary_fea_mat(unary_prob_img.height(), unary_prob_img.width());
  for (long r=0; r<local_unary_fea_mat.rows(); ++r) 
    for (long c=0; c<local_unary_fea_mat.cols(); ++c) { 
      long flat_idx = svm_struct_ss::util::flat_idx_rowcol( r,c,unary_prob_img.width() );
      local_unary_fea_mat(r,c) = unary_prob_img( c,r,y.flatten_label[flat_idx] ); 
    }

  // put the local unary joint feature to the global unary joint feature whose size is constant
  // the top left corner of this feature matrix always coincides with the top left corner of max feature matrix
  // the uncovered area is where the feature values are missing, for which we use the initial values
  // TODO @tttor should we apply log to the unary prob?
  // TODO @tttor: how to handle missing features: make them zero-valued?
  svm_struct_ss::util::FeatureMatrix unary_fea_mat;
  unary_fea_mat = svm_struct_ss::util::FeatureMatrix::Zero(SVM_STRUCT_SS_EXAMPLE_PARAM_MAX_IMAGE_HEIGHT,SVM_STRUCT_SS_EXAMPLE_PARAM_MAX_IMAGE_WIDTH);
  unary_fea_mat.topLeftCorner(local_unary_fea_mat.rows(),local_unary_fea_mat.cols()) = local_unary_fea_mat;
  debug_msg("got unary fea");

  // fill in the SVECTOR
  // Note: WORD's wnum begin at 1, therefore: wnum = word_idx + 1
  // TODO make the words sparse (?)
  FNUM word_idx = 0;
  for (FNUM i=0; i<unary_fea_mat.size(); ++i) {
    fvec->words[word_idx].wnum = word_idx + 1;
    fvec->words[word_idx].weight = unary_fea_mat.data()[i];
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

  assert(word_idx==(n_word-1) && "err: word_idx must point to the last element");
  debug_out_msg("joint_feature_extractor::psi");
}

}
}

#endif