#include <iostream>
#include <fstream>

#include <shotton/shotton.h>
#include <util/util.h>

int main(int argc, char* argv[]) {
  using namespace std;
  using namespace lab1231_sun_prj;

  assert(argc==6 && "argc!=6");

  // Set the dataset param
  shotton::DataParam data_param;
  data_param["name"] = argv[1];
  data_param["n_label"] = argv[2];
  data_param["ori_img_dir"] = argv[3];
  data_param["test_img_list_filepath"] = argv[4];

  // Train
  shotton::EnergyParam energy_param;
  shotton::train(data_param, &energy_param);

  // Annotate
  vector<string> test_img_filenames;
  
  ifstream test_img_list_file(data_param["test_img_list_filepath"].c_str());
  if (test_img_list_file.is_open()) {
    string line;
    while ( getline(test_img_list_file,line) ) {
      test_img_filenames.push_back(line);
    }
    test_img_list_file.close();
  }

  const string ann_results_dir = argv[5];
  std::vector<Eigen::MatrixXi> ann_results;

  for (size_t i=0; i<test_img_filenames.size(); ++i) {
    const string img_filename = test_img_filenames.at(i);
    cout << "ANNOTATING img_filename" << img_filename << endl;

    Eigen::MatrixXi ann;
    ann = shotton::annotate(img_filename, data_param, energy_param);

    const string ann_filepath = string(ann_results_dir+img_filename.substr(0,img_filename.size()-4)+".ann");
    util::csv_write<Eigen::MatrixXi>(ann, ann_filepath);
    
    // ann_results.push_back(ann);
    break;
  }

  return 0;
}