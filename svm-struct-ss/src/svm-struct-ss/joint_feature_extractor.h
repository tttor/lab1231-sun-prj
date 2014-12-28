#ifndef SVM_STRUCT_SS_JOINT_FEATURE_EXTRACTOR_H
#define SVM_STRUCT_SS_JOINT_FEATURE_EXTRACTOR_H

#include <svm_light/svm_common.h>
#include <vector>

#include <shotton/shotton.h>
#include <shotton/edge_potential.h>
#include <opencv2/core/core.hpp>

#include "util.h"
#include "example_param.h"
#include "io.h"

namespace svm_struct_ss {
namespace joint_feature_extractor {

void set_unary_fea_mat(const PATTERN& x, const LABEL& y, util::FeatureMatrix* unary_fea_mat) {
  using namespace std;

  // get unary Philipp prob image
  debug_msg("computing unary fea:...");
  ProbImage unary_prob_img;
  const string prob_img_filepath = string(data_param::unary_potential_dir+"/"+x.id+".unary");
  // debug_var("prob_img_filepath",prob_img_filepath);
  unary_prob_img.load(prob_img_filepath.c_str());
  // debug_msg("got unary_prob_img");

  // get unary joint feature
  util::FeatureMatrix local_unary_fea_mat(unary_prob_img.height(), unary_prob_img.width());
  for (size_t r=0; r<local_unary_fea_mat.rows(); ++r) 
    for (size_t c=0; c<local_unary_fea_mat.cols(); ++c) { 
      size_t flat_idx = util::flat_idx_rowcol( r,c,unary_prob_img.width() );
      local_unary_fea_mat(r,c) = unary_prob_img( c,r,y.flatten_label[flat_idx] ); 
    }

  // put the local unary joint feature to the global unary joint feature whose size is constant
  // the top left corner of this feature matrix always coincides with the top left corner of max feature matrix
  // the uncovered area is where the feature values are missing, for which we use the initial values
  // TODO @tttor should we apply log to the unary prob?
  // TODO @tttor: how to handle missing features: make them zero-valued?
  // util::FeatureMatrix unary_fea_mat;
  unary_fea_mat->topLeftCorner(local_unary_fea_mat.rows(),local_unary_fea_mat.cols()) = local_unary_fea_mat;
  debug_msg("got unary fea");
}

inline size_t pott_function(const int& node_label, const int& neighbor_node_label, 
                            const float& equal_pen, const float& unequal_pen) {
  if ((node_label==data_param::void_label) or (neighbor_node_label==data_param::void_label) )
    return equal_pen;

  if (node_label==neighbor_node_label) return equal_pen;
  else return unequal_pen;
}

void set_pairwise_fea_mat(const PATTERN& pattern, const LABEL& label, 
                          util::FeatureMatrix* horizontal_pairwise_fea_mat, 
                          util::FeatureMatrix* vertical_pairwise_fea_mat) {
  namespace sun = lab1231_sun_prj;
  using namespace std;

  assert(SVM_STRUCT_SS_EXAMPLE_PARAM_PAIRWISE_POTENTIAL_FUNCTION=="Pott" && "FATAL: unsupporter pairwise potential funct");

  //
  sun::shotton::DataParam data_param;
  data_param["name"] = svm_struct_ss::data_param::dataset;
  data_param["n_label"] = svm_struct_ss::data_param::n_label;
  data_param["ori_img_dir"] = svm_struct_ss::data_param::ori_jpg_dir;
  data_param["unary_potential_dir"] = svm_struct_ss::data_param::unary_potential_dir;

  //
  const string img_filename = pattern.id;
  const string img_path = string(data_param["ori_img_dir"]+"/"+img_filename+".jpg");
  cv::Mat img = cv::imread(img_path, CV_LOAD_IMAGE_COLOR);

  // TODO @tttor: may pre-compute this beta
  float beta;
  beta = sun::shotton::edge_potential::get_beta(img);

  //
  sun::shotton::EnergyParam energy_internal_param;
  sun::shotton::train(data_param, &energy_internal_param);

  Eigen::MatrixXd theta_phi(2, 1);
  theta_phi << energy_internal_param["theta_phi_1"], 
               energy_internal_param["theta_phi_2"];

  const float pott_equal_pen = 0.0;  

  //
  util::FeatureMatrix local_horizontal_pairwise_fea_mat(label.height, label.width);
  util::FeatureMatrix local_vertical_pairwise_fea_mat(label.height, label.width);

  for (size_t x=0; x<img.cols; ++x) {
    for (size_t y=0; y<img.rows; ++y) {
      cv::Point2i node;   
      node.x = x; 
      node.y = y;

      const int node_label = label.flatten_label[util::flat_idx_xy(node.x,node.y,img.cols)]; 

      // horizontal (to the right) connection:
      // (x, y) -- (x + 1, y) 
      if (x+1 < img.cols) {
        cv::Point2i neighbor_node;   
        neighbor_node.x = x+1; 
        neighbor_node.y = y;

        const int neighbor_node_label = label.flatten_label[util::flat_idx_xy(neighbor_node.x,neighbor_node.y,img.cols)];
        const float unequal_pen = sun::shotton::edge_potential::potential(img.at<cv::Vec3b>(node), 
                                                                          img.at<cv::Vec3b>(neighbor_node), 
                                                                          beta, theta_phi);

        local_horizontal_pairwise_fea_mat(y,x) = pott_function(node_label, neighbor_node_label, pott_equal_pen, unequal_pen);
      }

      // vertical (to the bottom (down)) connection:
      // (x, y) -- (x, y + 1)
      if (y+1 < img.rows) {
        cv::Point2i neighbor_node;   
        neighbor_node.x = x; 
        neighbor_node.y = y+1;

        const int neighbor_node_label = label.flatten_label[util::flat_idx_xy(neighbor_node.x,neighbor_node.y,img.cols)];
        const float unequal_pen = sun::shotton::edge_potential::potential(img.at<cv::Vec3b>(node), 
                                                                          img.at<cv::Vec3b>(neighbor_node), 
                                                                          beta, theta_phi);

        local_vertical_pairwise_fea_mat(y,x) = pott_function(node_label, neighbor_node_label, pott_equal_pen, unequal_pen);
      }
    }
  }

  // Overlay the local fea mat on top of global fea mat whose size is constant
  // The top left corners of both matrix coincide
  horizontal_pairwise_fea_mat->topLeftCorner(local_horizontal_pairwise_fea_mat.rows(),
                                             local_horizontal_pairwise_fea_mat.cols()) 
                                             = local_horizontal_pairwise_fea_mat;

  vertical_pairwise_fea_mat->topLeftCorner(local_vertical_pairwise_fea_mat.rows(),
                                           local_vertical_pairwise_fea_mat.cols()) 
                                           = local_vertical_pairwise_fea_mat;

}

void extract_feature(const PATTERN& x, const LABEL& y,const size_t& n_word, SVECTOR* fvec) {
  debug_in_msg("joint_feature_extractor::psi");
  using namespace std;

  //
  util::FeatureMatrix unary_fea_mat;
  unary_fea_mat = util::FeatureMatrix::Zero(util::example_max_height(),util::example_max_width());
  set_unary_fea_mat(x,y, &unary_fea_mat);

  // //
  // util::FeatureMatrix horizontal_pairwise_fea_mat;
  // horizontal_pairwise_fea_mat = util::FeatureMatrix::Zero(util::example_max_height(),util::example_max_width()-1);

  // util::FeatureMatrix vertical_pairwise_fea_mat;
  // vertical_pairwise_fea_mat = util::FeatureMatrix::Zero(util::example_max_height()-1,util::example_max_width());

  // set_pairwise_fea_mat(x,y,&horizontal_pairwise_fea_mat,&vertical_pairwise_fea_mat);

  // fill in the SVECTOR
  // Note: WORD's wnum begin at 1, therefore: wnum = word_idx + 1
  // TODO make the words sparse (?)
  FNUM word_idx = 0;
  for (FNUM i=0; i<unary_fea_mat.size(); ++i) {
    fvec->words[word_idx].wnum = word_idx + 1;
    fvec->words[word_idx].weight = unary_fea_mat.data()[i];
    ++word_idx;
  }

  // for (FNUM i=0; i<horizontal_pairwise_fea_mat.size(); ++i) {
  //   fvec->words[word_idx].wnum = word_idx + 1;
  //   fvec->words[word_idx].weight = horizontal_pairwise_fea_mat.data()[i];
  //   ++word_idx;
  // }

  // for (FNUM i=0; i<vertical_pairwise_fea_mat.size(); ++i) {
  //   fvec->words[word_idx].wnum = word_idx + 1;
  //   fvec->words[word_idx].weight = vertical_pairwise_fea_mat.data()[i];
  //   ++word_idx;
  // }

  assert(word_idx==(n_word-1) && "err: word_idx must node to the last element");
  debug_out_msg("joint_feature_extractor::psi");
}

}
}

#endif
