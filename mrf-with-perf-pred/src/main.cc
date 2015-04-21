#include <iostream>
#include <boost/lexical_cast.hpp>
#include <opencv2/opencv.hpp>

#include <util/util.h>
#include <mrf_with_perf_pred/mrf_with_perf_pred.h>

int main(int argc, char* argv[]) {
  using namespace std;
  namespace sun = lab1231_sun_prj;
  namespace sun_mrf = lab1231_sun_prj::mrf_with_perf_pred;

  sun::util::DataParam data_param;
  sun::util::EnergyParam energy_param;

  if (argc == 12) {
    data_param["dataset_name"] = argv[1];
    data_param["n_label"] = argv[2];
    data_param["img_dir"] = argv[3];
    data_param["img_extension"] = argv[4];
    data_param["img_id_list_filepath"] = argv[5];
    data_param["unary_philipp_dir"] = argv[6];
    data_param["segmentation_dir"] = argv[7];
    data_param["segmentation_param"] = argv[8];
    data_param["knowledge_dir"] = argv[9];
    data_param["trained_estimator_filepath"] = argv[10];
    data_param["result_dir"] = argv[11];

    energy_param["theta_phi_1"] = 4.5;
    energy_param["theta_phi_2"] = 1;

    sun_mrf::PerformancePredictor::tmp_dir = string(data_param["result_dir"]+"/tmp");
    sun_mrf::PerformancePredictor::ypred_filepath = string(data_param["result_dir"]+"/tmp/y_pred.tmp");
    sun_mrf::PerformancePredictor::knowledge_dir = data_param["knowledge_dir"];
    sun_mrf::PerformancePredictor::trained_estimator_filepath = data_param["trained_estimator_filepath"];
    sun_mrf::PerformancePredictor::ori_img_dir = data_param["img_dir"];
    sun_mrf::PerformancePredictor::ori_img_extension = data_param["img_extension"];
  }
  else {
    assert(false && "UNSUFFICIENT ARGUMENTS!");
  }

  // Test: Annotate
  vector<string> img_id_list;
  img_id_list = sun::util::read_list(data_param["img_id_list_filepath"]);

  for (size_t i=0; i<img_id_list.size(); ++i) {
    const string img_id = img_id_list.at(i);
    cout << "ANNOTATING: " << img_id << " (" << i+1 << "/" << img_id_list.size() << ")" << endl;

    Eigen::MatrixXi ann;
    ann = sun_mrf::annotate(img_id, data_param, energy_param);

    const string ann_filepath = string(data_param["result_dir"]+"/"+img_id+".csv");
    const string ann_img_filepath = string(data_param["result_dir"]+"/"+img_id+data_param["img_extension"]);
    sun::util::csv_write<Eigen::MatrixXi>(ann, ann_filepath);
    cv::imwrite(ann_img_filepath, sun::util::ann2img(ann, data_param["dataset_name"]));
  }

  return 0;
}
