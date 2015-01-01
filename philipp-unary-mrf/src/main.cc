#include <philipp-unary-mrf/philipp_unary_mrf.h>


int main(int argc, char* argv[]) {
  using namespace std;
  namespace sun = lab1231_sun_prj;

  sun::util::DataParam data_param;
  if (argc == 7) {
    //
    data_param["dataset_name"] = argv[1];
    data_param["n_label"] = argv[2];
    data_param["ori_img_dir"] = argv[3];
    data_param["test_img_list_filepath"] = argv[4];
    data_param["result_dir"] = argv[5];
    data_param["unary_philipp_dir"] = argv[6];
    // 
    
  }
  else {
    assert(false && "UNSUFFICIENT ARGUMENTS!");
  }
  cout << data_param["test_img_list_filepath"] << endl;

  // Test: Annotate
  vector<string> test_img_filenames;
  test_img_filenames = sun::util::read_list(data_param["test_img_list_filepath"]);
  
  for (size_t i=0; i<test_img_filenames.size(); ++i) {
    const string img_filename = test_img_filenames.at(i);
    cout << "ANNOTATING: " << img_filename << " (" << i+1 << "/" << test_img_filenames.size() << ")" << endl;

    Eigen::MatrixXi ann;
    ann = sun::philipp_unary_mrf::annotate(img_filename, data_param);

    const string ann_csv_filepath = string(data_param["result_dir"]+"/"+img_filename+".csv");
    // const string ann_img_filepath = string(data_param["result_dir"]+img_filename+".bmp");
    sun::util::csv_write<Eigen::MatrixXi>(ann, ann_csv_filepath);
    // cv::imwrite(ann_img_filepath, sun::util::ann2img(ann, data_param["dataset_name"]));
  }

  return 0;
}