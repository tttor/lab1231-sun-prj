#include <iostream>
#include <boost/lexical_cast.hpp>
#include <util/util.h>
#include <ladicky/ladicky.h>
#include <majority/majority.h>

int main(int argc, char* argv[]) {
  using namespace std;
  namespace sun = lab1231_sun_prj;

  sun::util::DataParam data_param;
  sun::util::EnergyParam energy_param;
  cout << argc<< endl;
  if (argc == 10) {
    //
    data_param["dataset_name"] = argv[1];
    data_param["n_label"] = argv[2];
    data_param["ori_img_dir"] = argv[3];
    data_param["test_img_list_filepath"] = argv[4];
    data_param["result_dir"] = argv[5];
    data_param["unary_philipp_dir"] = argv[6];
    data_param["superpixel_dir"] = argv[7];
    data_param["param_superpixel_list"] = argv[8];
    data_param["final_result_dir"] = argv[9];
  }
  else {
    assert(false && "UNSUFFICIENT ARGUMENTS!");
  }

  // Test: Annotate
  vector<string> test_img_filenames;
  test_img_filenames = sun::util::read_list(data_param["test_img_list_filepath"]);
  
  vector<string> param_superpixel_list;
  param_superpixel_list = sun::util::read_list(data_param["param_superpixel_list"]);

  for (size_t i=0; i<test_img_filenames.size(); ++i) {
    //file name processing
    const string img_filename = test_img_filenames.at(i).substr(0,test_img_filenames.at(i).size()-4);      
    cv::Mat img = cv::imread(data_param["ori_img_dir"] + test_img_filenames.at(i), CV_LOAD_IMAGE_COLOR);

    int n_label = atoi(data_param["n_label"].c_str()); 
    size_t*** buffer_result = sun::majority::init_buffer_result(img.rows, img.cols, n_label);
    
    //annotate per superpixel
    for (size_t j=0; j<param_superpixel_list.size(); ++j) { 
      char buffer [50];
      sprintf(buffer, "%s-%s", img_filename.c_str(), param_superpixel_list.at(j).c_str() );
      const string filename = string(buffer);
      const string superpixel_filename = filename + ".sup";
           
      cout << "ANNOTATING: " << filename << ".bmp - (" << i*param_superpixel_list.size()+j+1 << "/" << test_img_filenames.size() * param_superpixel_list.size() << ")" << endl;
      Eigen::MatrixXi ann;
      ann = sun::ladicky::annotate(img_filename+".bmp", img_filename+"/"+superpixel_filename, data_param, energy_param);

      buffer_result = sun::majority::add_result_to_buffer(buffer_result, ann, img.rows, img.cols);

      const string ann_filepath = string(data_param["result_dir"]+filename+".ann");
      const string ann_img_filepath = string(data_param["result_dir"]+filename+".bmp");

      sun::util::csv_write<Eigen::MatrixXi>(ann, ann_filepath);
      cv::imwrite(ann_img_filepath, sun::util::ann2img(ann, data_param["dataset_name"]));
    }

    const string ann_filepath_final = string(data_param["final_result_dir"]+img_filename+".ann");
    const string ann_img_filepath_final = string(data_param["final_result_dir"]+img_filename+".bmp");    

    Eigen::MatrixXi final_ann = sun::majority::get_final_result_from_buffer(buffer_result, img.rows, img.cols, n_label);
    cv::imwrite(ann_img_filepath_final, sun::util::ann2img(final_ann, data_param["dataset_name"]));
    sun::util::csv_write<Eigen::MatrixXi>(final_ann, ann_filepath_final);
  }

  return 0;
}