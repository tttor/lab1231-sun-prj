#include <iostream>
#include <boost/lexical_cast.hpp>
#include <util/util.h>
#include <ladicky/ladicky.h>
#include <opencv2/opencv.hpp>

int main(int argc, char* argv[]) {
  using namespace std;
  namespace sun = lab1231_sun_prj;

  sun::util::DataParam data_param;
  sun::util::EnergyParam energy_param;

  if (argc == 9) {
    //
    data_param["dataset_name"] = argv[1];
    data_param["n_label"] = argv[2];
    data_param["ori_img_dir"] = argv[3];
    data_param["test_img_list_filepath"] = argv[4];
    data_param["result_dir"] = argv[5];
    data_param["unary_philipp_dir"] = argv[6];
    data_param["superpixel_dir"] = argv[7];
    data_param["param_superpixel"] = argv[8];

    energy_param["theta_phi_1"] = 4.5;
    energy_param["theta_phi_2"] = 1;
  }
  else {
    assert(false && "UNSUFFICIENT ARGUMENTS!");
  }

  // Test: Annotate
  vector<string> img_filenames;
  img_filenames = sun::util::read_list(data_param["test_img_list_filepath"]);
  
  for (size_t i=0; i<img_filenames.size(); ++i) {
    const string img_filename = img_filenames.at(i);

    string superpixel_filename = string(img_filename) + 
                                  "/" + string(img_filename) + 
                                  data_param["param_superpixel"] + ".sup";
                                  
    cout << "ANNOTATING: " << img_filename 
                           << " (" << i+1 << "/" << img_filenames.size() << ")" << endl;

    Eigen::MatrixXi ann;
    ann = sun::ladicky::annotate(img_filename, superpixel_filename, data_param, energy_param);

    const string ann_filepath = string(data_param["result_dir"]+img_filename+".ann");
    const string ann_img_filepath = string(data_param["result_dir"]+img_filename+".bmp");
    sun::util::csv_write<Eigen::MatrixXi>(ann, ann_filepath);
    cv::imwrite(ann_img_filepath, sun::util::ann2img(ann, data_param["dataset_name"]));
  }

  return 0;
}