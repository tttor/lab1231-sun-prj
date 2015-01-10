#include <philipp-unary-mrf/philipp_unary_mrf.h>

namespace lab1231_sun_prj {
namespace philipp_unary_mrf{

Eigen::MatrixXi annotate(const std::string& img_filename, util::DataParam data_param) {
  using namespace std;
  // std::cout << "annotate(): BEGIN\n";

  const string img_path = string(data_param["ori_img_dir"]+"/"+img_filename+".jpg");
  cout << "img_path= " << img_path << endl;

  cv::Mat img = cv::imread(img_path, CV_LOAD_IMAGE_COLOR);
  cout << "img.cols= " << img.cols << endl;
  cout << "img.rows= " << img.rows << endl;

  const size_t n_var = img.rows * img.cols;
  const size_t n_label = boost::lexical_cast<size_t>(data_param["n_label"]);

  GraphicalModel gm( opengm::SimpleDiscreteSpace<size_t, size_t>(n_var, n_label) );
  set_1st_order(img_filename, n_label, data_param["unary_philipp_dir"], &gm);

  Eigen::MatrixXi ann(img.rows, img.cols);
  const string method = "AlphaExpansion";//: "AlphaExpansion", 
  infer(method, gm, n_var, &ann);

  std::cout << "annotate(): END\n";
  return ann;
}

double get_unary_potential_from_unary_prob_image(const ProbImage& prob_img, 
                                                 const size_t& x, const size_t& y,
                                                 const size_t& label) {
  double potential;
  const double eps = 0.0000000001;// for avoiding log(0)
  potential = -1.0 * log( prob_img(x,y,label)+eps );// multiplied by -1.0 for E>=0 since (log p)<=0

  return potential;
}

void set_1st_order(const std::string& img_filename, const size_t& n_label, 
                   const std::string& unary_prob_img_dir, 
                   GraphicalModel* gm) {
  using namespace std;

  ProbImage unary_prob_img;
  // const string unary_prob_img_path = string( unary_prob_img_dir+"/"+img_filename+".c_unary" );
  // unary_prob_img.decompress( unary_prob_img_path.c_str() );
  const string unary_prob_img_path = string( unary_prob_img_dir+"/"+img_filename+".unary" );
  unary_prob_img.load( unary_prob_img_path.c_str() );
  // cout << "unary_prob_img_path= " << unary_prob_img_path << endl;

  // cout << "unary_prob_img.depth()= " << unary_prob_img.depth() << endl;
  // cout << "unary_prob_img.width()= " << unary_prob_img.width() << endl;
  // cout << "unary_prob_img.height()= " << unary_prob_img.height() << endl;

  for (size_t x=0; x<unary_prob_img.width(); ++x) {
    for (size_t y=0; y<unary_prob_img.height(); ++y) {
      // add a function
      const size_t shape[] = {n_label};
      opengm::ExplicitFunction<double> energy(shape, shape+1);

      for(int i = 0; i < n_label; i++) {
        energy(i) = get_unary_potential_from_unary_prob_image(unary_prob_img,x,y,i);
      }

      GraphicalModel::FunctionIdentifier fid = gm->addFunction(energy);
      
      // add a factor
      size_t flat_idxes[] = {util::flat_idx_xy(x, y, unary_prob_img.width())};
      gm->addFactor(fid, flat_idxes, flat_idxes+1);
    }
  }
}

void infer(const std::string& method, const GraphicalModel& gm, const size_t& n_var, Eigen::MatrixXi* ann) {
  using namespace std;
  cout << "infer(): BEGIN\n";
  cout << "method= " << method;

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

    cout << " is inferring ..." << endl;
    inf_engine.infer();
    inf_engine.arg(ann_vec);
  }
  else if (method=="ICM") {
    typedef opengm::ICM<GraphicalModel, opengm::Minimizer> IcmType;
    IcmType::VerboseVisitorType visitor;

    cout << "is inferring ..." << endl;
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

}
}