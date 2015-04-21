#ifndef LAB1231_SUN_PRJ_PERF_PREDICTOR_H
#define LAB1231_SUN_PRJ_PERF_PREDICTOR_H

#include <util/util.h>
#include <boost/lexical_cast.hpp>

#include <boost/python.hpp> //boost libraries
#include <Python.h> //python libraries
namespace bp = boost::python;

namespace lab1231_sun_prj {
namespace mrf_with_perf_pred {

class PerformancePredictor {
 public:
  static double predict(const int* ann);

  
  static std::string tmp_dir;
  static std::string knowledge_dir;
  static std::string trained_estimator_filepath;
  static std::string ori_img_dir;
  static std::string ori_img_extension;
  static std::string current_img_id;
  static std::string ypred_filepath;

  static size_t current_img_nrow;
  static size_t current_img_ncol;

 private:
  static double read_perf_txt(const std::string& filepath);
};

}
}

#endif
