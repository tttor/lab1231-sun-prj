#include "main.h"

int main(int argc, char* argv[]) {
  using namespace std;

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
    ann = annotate(img_filename, data_param);

    const string ann_filepath = string(data_param["result_dir"]+img_filename+".csv");
    sun::util::csv_write<Eigen::MatrixXi>(ann, ann_filepath);
    // const string ann_img_filepath = string(data_param["result_dir"]+img_filename.substr(0,img_filename.size()-4)+".bmp");
    // cv::imwrite(ann_img_filepath, sun::util::ann2img(ann, data_param["dataset_name"]));
  }

  return 0;
}

Eigen::MatrixXi annotate(const std::string& img_filename, sun::util::DataParam data_param) {
  using namespace std;
  // std::cout << "annotate(): BEGIN\n";

  const string img_path = string(data_param["ori_img_dir"] + img_filename + ".jpg");
  // cout << "img_path= " << img_path << endl;

  cv::Mat img = cv::imread(img_path, CV_LOAD_IMAGE_COLOR);
  // cout << "img.cols= " << img.cols << endl;
  // cout << "img.rows= " << img.rows << endl;

  const size_t n_var = img.rows * img.cols;
  const size_t n_label = boost::lexical_cast<size_t>(data_param["n_label"]);

  GraphicalModel gm( opengm::SimpleDiscreteSpace<size_t, size_t>(n_var, n_label) );

  set_1st_order(img_filename, n_label, data_param["unary_philipp_dir"], &gm);

  Eigen::MatrixXi ann(img.rows, img.cols);
  const string method = "AlphaExpansion";//: "AlphaExpansion", 
  infer(method, gm, n_var, &ann);

  // std::cout << "annotate(): END\n";
  return ann;
}

void set_1st_order(const std::string& img_filename, const size_t& n_label, const std::string& unary_prob_img_dir, GraphicalModel* gm) {
  using namespace std;

  const string unary_prob_img_path = string( unary_prob_img_dir+img_filename+".c_unary" );
  // cout << "unary_prob_img_path= " << unary_prob_img_path << endl;

  ProbImage unary_prob_img;
  unary_prob_img.decompress( unary_prob_img_path.c_str() );

  // cout << "unary_prob_img.depth()= " << unary_prob_img.depth() << endl;
  // cout << "unary_prob_img.width()= " << unary_prob_img.width() << endl;
  // cout << "unary_prob_img.height()= " << unary_prob_img.height() << endl;

  for (size_t x=0; x<unary_prob_img.width(); ++x) {
    for (size_t y=0; y<unary_prob_img.height(); ++y) {
      // add a function
      const size_t shape[] = {n_label};
      opengm::ExplicitFunction<double> energy(shape, shape+1);

      for(int i = 0; i < n_label; i++) 
        energy(i) = -1 * unary_prob_img(x,y,i);

      GraphicalModel::FunctionIdentifier fid = gm->addFunction(energy);
      
      // add a factor
      size_t flat_idxes[] = {sun::util::flat_idx(x, y, unary_prob_img.width())};
      gm->addFactor(fid, flat_idxes, flat_idxes+1);
    }
  }
}

void infer(const std::string& method, const GraphicalModel& gm, const size_t& n_var, Eigen::MatrixXi* ann) {
  using namespace std;
  // cout << "infer(): BEGIN\n";
  // cout << "method= " << method << endl;

  vector<size_t> ann_vec(n_var);
  
  if (method=="AlphaExpansion") {
    typedef 
    opengm::external::MinSTCutKolmogorov<size_t, double> 
    MinStCutType;

    typedef 
    opengm::GraphCut<GraphicalModel, opengm::Minimizer, MinStCutType> 
    MinGraphCut;
    
    typedef 
    opengm::AlphaExpansion<GraphicalModel, MinGraphCut> 
    MinAlphaExpansion;

    MinAlphaExpansion inf_engine(gm);

    cout << "Inferring with " << method << "..." << endl;
    inf_engine.infer();
    inf_engine.arg(ann_vec);
  }
  else if (method=="ICM") {
    typedef opengm::ICM<GraphicalModel, opengm::Minimizer> IcmType;
    IcmType::VerboseVisitorType visitor;

    IcmType inf_engine(gm);
    inf_engine.infer(visitor);
  }
  else {
    assert(false && "Unknown inference method");
  }

  //
  size_t idx = 0;
  for (size_t i=0; i<ann->rows(); ++i) {
    for (size_t j=0; j<ann->cols(); ++j) {
      (*ann)(i,j) = ann_vec.at(idx);
      ++ idx;
    }
  }

  std::cout << "infer(): END\n";
}