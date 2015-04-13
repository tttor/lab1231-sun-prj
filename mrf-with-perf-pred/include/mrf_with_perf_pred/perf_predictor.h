#ifndef LAB1231_SUN_PRJ_PERF_PREDICTOR_H
#define LAB1231_SUN_PRJ_PERF_PREDICTOR_H

#include <boost/python.hpp> //boost libraries
#include <Python.h> //python libraries
namespace bp = boost::python;

namespace lab1231_sun_prj {
namespace mrf_with_perf_pred {

class PerformancePredictor {
 public:
  static double predict();

  static std::string hot_ann_dir;
  static std::string knowledge_dir;
  static std::string trained_estimator_filepath;

 private:
  static double read_perf_txt();
};

}
}

#endif
