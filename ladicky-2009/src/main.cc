#include <iostream>
#include <boost/lexical_cast.hpp>
#include <util/util.h>
#include <ladicky/ladicky.h>

int main(int argc, char* argv[]) {
  using namespace std;
  namespace sun = lab1231_sun_prj;

  sun::util::DataParam data_param;
  sun::util::EnergyParam energy_param;

  if (argc > 1) {
    //
    data_param["dataset_name"] = argv[1];
    data_param["n_label"] = argv[2];
    data_param["ori_img_dir"] = argv[3];
    data_param["test_img_list_filepath"] = argv[4];
    data_param["result_dir"] = argv[5];
    data_param["unary_philipp_dir"] = argv[6];

    // Train
    energy_param["SLIC_region"] = boost::lexical_cast<double>(argv[7]);
    energy_param["SLIC_regularization"] = boost::lexical_cast<double>(argv[8]);
    energy_param["SLIC_min_region"] = boost::lexical_cast<double>(argv[9]);
  }
  else {
    assert(false && "UNSUFFICIENT ARGUMENTS!");
  }

  // Test: Annotate
  vector<string> test_img_filenames;
  test_img_filenames = sun::util::read_list(data_param["test_img_list_filepath"]);
  
  for (size_t i=0; i<test_img_filenames.size(); ++i) {
    const string img_filename = test_img_filenames.at(i);
    cout << "ANNOTATING: " << img_filename << " (" << i+1 << "/" << test_img_filenames.size() << ")" << endl;

    Eigen::MatrixXi ann;
    ann = sun::ladicky::annotate(img_filename, data_param, energy_param);

    const string ann_filepath = string(data_param["result_dir"]+img_filename.substr(0,img_filename.size()-4)+".ann");
    const string ann_img_filepath = string(data_param["result_dir"]+img_filename.substr(0,img_filename.size()-4)+".bmp");
    sun::util::csv_write<Eigen::MatrixXi>(ann, ann_filepath);
    cv::imwrite(ann_img_filepath, sun::util::ann2img(ann, data_param["dataset_name"]));
  }

  return 0;
}