#ifndef LAB1231_SUN_PRJ_LEAR_GIST_WRAPPER_H_
#define LAB1231_SUN_PRJ_LEAR_GIST_WRAPPER_H_

#include <iostream>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <eigen3/Eigen/Dense>
#include <util/eigenlib_support.h>
#include "lear_gist-1.2/gist.h"

namespace lab1231_sun_prj {

class LearGistWrapper {
 public:
  //! Extract GIST features
  static void extract(const std::string& img_path, const std::string& out_dir_path);
  
  //! Copied from compute_gist.c of Lear GIST implementation
  static color_image_t *load_ppm(const char *fname);
  
 private:
};

}// namespace lab1231_sun_prj

#endif
