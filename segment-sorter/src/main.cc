#include <iostream>
#include <boost/lexical_cast.hpp>
#include <util/util.h>
#include <ladicky/ladicky.h>

#include <unistd.h>
#include <sys/time.h>

int main(int argc, char* argv[]) {
  using namespace std;
  namespace sun = lab1231_sun_prj;

  sun::util::DataParam data_param;
  sun::util::EnergyParam energy_param;
  
  struct timeval start, end;
  long mtime, seconds, useconds;  

  if (argc == 8) {
    //
    data_param["dataset_name"] = argv[1];
    data_param["n_label"] = argv[2];
    data_param["ori_img_dir"] = argv[3];
    data_param["test_img_list_filepath"] = argv[4];
    data_param["result_dir"] = argv[5];
    data_param["unary_philipp_dir"] = argv[6];
    data_param["superpixel_dir"] = argv[7];

    energy_param["theta_phi_1"] = 4.5;
    energy_param["theta_phi_2"] = 1;
  }
  else {
    assert(false && "UNSUFFICIENT ARGUMENTS!");
  }

  // Test: Annotate
  vector<string> test_img_filenames;
  test_img_filenames = sun::util::read_list(data_param["test_img_list_filepath"]);
  
  for (size_t i=0; i<test_img_filenames.size(); ++i) {
    //file name processing
    const string img_filename = test_img_filenames.at(i) + ".bmp";
    // if (data_param["dataset_name"] == "msrc"){
    //   img_filename = test_img_filenames.at(i) + ".bmp";
    // } else {
    //   img_filename = test_img_filenames.at(i) + ".jpg";
    // }

    cv::Mat img = cv::imread(data_param["ori_img_dir"] + img_filename, CV_LOAD_IMAGE_COLOR);

    int n_label = atoi(data_param["n_label"].c_str()); 
    
    //annotate per superpixel
    const string filename = test_img_filenames.at(i);
         
    cout << "ANNOTATING: " << filename << ".bmp - (" << i << "/" << test_img_filenames.size() << ")" << endl;
    Eigen::MatrixXi ann;

    gettimeofday(&start, NULL);
    ann = sun::ladicky::annotate(img_filename, filename+".sup", data_param, energy_param);
    gettimeofday(&end, NULL);

    seconds  = end.tv_sec  - start.tv_sec;
    useconds = end.tv_usec - start.tv_usec;
    mtime = ((seconds) * 1000 + useconds/1000.0) + 0.5;

    cout << "Elapsed time for annotate" << filename << " " << mtime <<" milliseconds\n";

    const string ann_filepath = string(data_param["result_dir"]+filename+".ann");
    const string ann_img_filepath = string(data_param["result_dir"]+filename+".bmp");

    sun::util::csv_write<Eigen::MatrixXi>(ann, ann_filepath);
    cv::imwrite(ann_img_filepath, sun::util::ann2img(ann, data_param["dataset_name"]));
  }

  return 0;
}