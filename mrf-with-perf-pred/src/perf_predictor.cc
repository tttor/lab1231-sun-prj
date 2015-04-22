#include <mrf_with_perf_pred/perf_predictor.h>
#include <iostream>

namespace sun = lab1231_sun_prj;
namespace sun_mrf = lab1231_sun_prj::mrf_with_perf_pred;

std::string sun_mrf::PerformancePredictor::current_img_id = "";
size_t sun_mrf::PerformancePredictor::current_img_nrow = 0;
size_t sun_mrf::PerformancePredictor::current_img_ncol = 0;

std::string sun_mrf::PerformancePredictor::tmp_dir = "";
std::string sun_mrf::PerformancePredictor::predictor_addr = "";

double sun_mrf::PerformancePredictor::read_perf_txt(const std::string& filepath) {
  using namespace std;
  
  vector<string> lines;

  ifstream file(filepath.c_str());
  if (file.is_open()) {
    string line;
    while ( getline(file,line) ) {
      const string last = line.substr(line.size()-1);
      if (last=="\r" or last=="\n") line = line.substr(0,line.size()-1);
      lines.push_back(line);
    }
    file.close();
  }
  else {
    cerr << "FATAL: file.is_open(): FALSE\n";
  }

  assert(lines.size()==1 && "lines.size()!=1");
  return boost::lexical_cast<double>(lines.at(0));
}

double sun_mrf::PerformancePredictor::predict(const int* ann_flat) {
  using namespace std;
  cout << "predict_performance(): BEGIN\n";
  
  //
  const string ann_filepath = string(tmp_dir+"/"+current_img_id+".csv");
  Eigen::MatrixXi ann = sun::util::arr2mat(ann_flat, current_img_nrow, current_img_ncol);

  sun::util::csv_write<Eigen::MatrixXi>(ann, ann_filepath);

  //
  bool succeed = true;
  try {
    Py_Initialize();

    bp::object predictor_client = bp::import("perf_predictor_client");
    predictor_client.attr("predict")(current_img_id, ann_filepath, 
                                                 tmp_dir, predictor_addr);
  }
  catch (bp::error_already_set const&) {
    succeed = false;
    PyErr_Print();
    cerr << "ERROR: predict_performance(): failed.\n";
  }

  //
  double perf_hat = 0.0;

  const string ypred_filepath = string(tmp_dir+"/ypred.csv");
  if (succeed) {
    perf_hat = read_perf_txt(ypred_filepath);
  }
  cout << "perf_hat= " << perf_hat << endl;

  cout << "predict_performance(): END\n";
  return perf_hat;
}
