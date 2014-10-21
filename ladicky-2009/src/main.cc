#include <iostream>
#include <util/util.h>
#include <ladicky/ladicky.h>

int main(int argc, char* argv[]) {
  using namespace std;
  namespace sun = lab1231_sun_prj;

  //
  sun::util::DataParam data_param;
  data_param["dataset_name"] = string("msrc");
  data_param["n_label"] = string("21");
  data_param["ori_img_dir"] = string("/home/tor/sun3/dataset/msrc/spt/Images/");
  data_param["test_img_list_filepath"] = string("/home/tor/sun4/exp/ladicky-2009-mrsc/meta/test_img.list");
  data_param["ann_result_dir"] = string("/home/tor/sun4/exp/ladicky-2009-mrsc/ann-result/");
  data_param["unary_philipp_dir"] = string("/home/tor/sun3/dataset/msrc/unary_philipp/msrc_compressed/");

  // Train
  sun::util::EnergyParam energy_param;
  energy_param["SLIC_region"] = 20;
  energy_param["SLIC_regularization"] = 1000;
  energy_param["SLIC_min_region"] = 10;

  // Test: Annotate
  vector<string> test_img_filenames;
  test_img_filenames = sun::util::read_list(data_param["test_img_list_filepath"]);
  
  for (size_t i=0; i<test_img_filenames.size(); ++i) {
    const string img_filename = test_img_filenames.at(i);
    cout << "ANNOTATING: " << img_filename << " (" << i+1 << "/" << test_img_filenames.size() << ")" << endl;

    Eigen::MatrixXi ann;
    ann = sun::ladicky::annotate(img_filename, data_param, energy_param);

    const string ann_filepath = string(data_param["ann_result_dir"]+img_filename.substr(0,img_filename.size()-4)+".ann");
    const string ann_img_filepath = string(data_param["ann_result_dir"]+img_filename.substr(0,img_filename.size()-4)+".bmp");
    sun::util::csv_write<Eigen::MatrixXi>(ann, ann_filepath);
    cv::imwrite(ann_img_filepath, sun::util::ann2img(ann, data_param["dataset_name"]));
  }

  return 0;
}