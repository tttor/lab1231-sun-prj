#include <mrf_with_perf_pred/perf_predictor.h>
#include <iostream>

namespace sun = lab1231_sun_prj;
namespace sun_mrf = lab1231_sun_prj::mrf_with_perf_pred;

std::string sun_mrf::PerformancePredictor::hot_ann_dir = "";
std::string sun_mrf::PerformancePredictor::knowledge_dir = "";
std::string sun_mrf::PerformancePredictor::trained_estimator_filepath = "";

double sun_mrf::PerformancePredictor::read_perf_txt() {
  //TODO
  return 0.0;
}

double sun_mrf::PerformancePredictor::predict() {
  using namespace std;
  cout << "predict_performance(): BEGIN\n";

  //
  bool succeed = true;
  try {
    Py_Initialize();

    bp::object predictor = bp::import("perf_predictor");
    predictor.attr("predict")(hot_ann_dir, knowledge_dir, trained_estimator_filepath);
  }
  catch (bp::error_already_set const&) {
    succeed = false;
    PyErr_Print();
  }

  //
  double perf_hat = 0.0;
  if (succeed) {
    perf_hat = read_perf_txt();
  }

  cout << "predict_performance(): END\n";
  return perf_hat;
}
