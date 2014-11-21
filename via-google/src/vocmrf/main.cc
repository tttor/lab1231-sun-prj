#include <iostream>
#include <fstream>
#include <stdlib.h>

#include <shotton/shotton.h>
#include <util/util.h>

int main(int argc, char* argv[]) {
  using namespace std;
  using namespace lab1231_sun_prj;

  // assert(argc==6 && "argc!=6");

  // Set the dataset param
  const string datasets_name = argv[1];
  const size_t n_label = atoi(argv[2]);
  const string img_dir = argv[3];
  const string unary_dir = argv[4];
  const string ann_dir = argv[5];
  const string ann_img_dir = argv[6];

  // Train
  shotton::EnergyParam energy_param;
  shotton::train(datasets_name, &energy_param);
  Eigen::MatrixXi ann = shotton::annotate(n_label, img_dir, unary_dir, energy_param);
  /*for (size_t i=0; i<ann.rows(); ++i) {
    for (size_t j=0; j<ann.cols(); ++j) {
      printf("%d",ann(i,j));
    }
  }*/
  util::csv_write(ann, ann_dir);
  cv::Mat output = util::ann2img(ann,"msrc");
  imwrite(ann_img_dir,output);


  // Annotate
  /*vector<string> test_img_filenames;
  
  ifstream test_img_list_file(data_param["test_img_list_filepath"].c_str());
  if (test_img_list_file.is_open()) {
    string line;
    while ( getline(test_img_list_file,line) ) {
      test_img_filenames.push_back(line);
    }
    test_img_list_file.close();
  }*/

  // const string ann_results_dir = argv[5];//"/home/tor/sun4/exp/rep-shotton-msrc/ann-csv/";
  // std::vector<Eigen::MatrixXi> ann_results;

/*  for (size_t i=0; i<test_img_filenames.size(); ++i) {
    const string img_filename = test_img_filenames.at(i);
    cout << "ANNOTATING img_filename" << img_filename << endl;
*/
    
    
    // ann_results.push_back(ann);
    /*break;
  }*/

  return 0;
}