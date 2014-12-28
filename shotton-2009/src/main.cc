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
  shotton::EnergyParam internal_energy_param;
  shotton::train(data_param, &internal_energy_param);

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

    // Dummy empty weight matrices
    Eigen::MatrixXd unary_weight = Eigen::MatrixXd::Ones(0,0);
    Eigen::MatrixXd h_pairwise_weight = Eigen::MatrixXd::Ones(0,0);
    Eigen::MatrixXd v_pairwise_weight = Eigen::MatrixXd::Ones(0,0);

    bool used_as_loss_augmented_inference = false;

    Eigen::MatrixXi ann;
    ann = shotton::annotate(img_filename, data_param, internal_energy_param, 
                            unary_weight, h_pairwise_weight, v_pairwise_weight,
                            used_as_loss_augmented_inference);

    const string ann_filepath = string(ann_results_dir+img_filename+".ann");
    util::csv_write<Eigen::MatrixXi>(ann, ann_filepath);
    
    // ann_results.push_back(ann);
    break;
  }

  return 0;
}