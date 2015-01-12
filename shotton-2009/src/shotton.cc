#include <shotton/shotton.h>

namespace lab1231_sun_prj {
namespace shotton {

void train(DataParam data_param, EnergyParam* energy_param) {
  const std::string data_name = data_param["name"];

  if (data_param["name"]=="msrc21") {
    // std::cout << "Use param from [Shotton,2009]\n";
    (*energy_param)["theta_phi_1"] = 4.5;
    (*energy_param)["theta_phi_2"] = 1.0;
  } 
  else if (data_param["name"]=="voc2010") {
    // std::cout << "Use param from [Shotton,2009]\n";
    (*energy_param)["theta_phi_1"] = 4.5;
    (*energy_param)["theta_phi_2"] = 1.0;
  }
  else {
    assert(false && "Unknown dataset!");
  }
}

Eigen::MatrixXi annotate(const std::string& img_filename, DataParam data_param, EnergyParam energy_param, 
                         const Eigen::MatrixXd& given_unary_weight, 
                         const Eigen::MatrixXd& given_horizontal_pairwise_weight, 
                         const Eigen::MatrixXd& given_vertical_pairwise_weight, 
                         const bool& used_as_loss_augmented_inference) {
  using namespace std;
  // debug_in_msg("shotton::annotate");

  const string img_path = string(data_param["ori_img_dir"]+"/"+img_filename+".jpg");
  cv::Mat img = cv::imread(img_path, CV_LOAD_IMAGE_COLOR);

  const size_t n_var = img.rows * img.cols;
  const size_t n_label = boost::lexical_cast<size_t>(data_param["n_label"]);

  GraphicalModel gm( opengm::SimpleDiscreteSpace<size_t, size_t>(n_var, n_label) );

  Eigen::MatrixXd unary_weight;
  if (unary_weight.size()==0) unary_weight = Eigen::MatrixXd::Ones(img.rows,img.cols);
  else unary_weight = given_unary_weight;

  Eigen::MatrixXd horizontal_pairwise_weight;
  if (horizontal_pairwise_weight.size()==0) horizontal_pairwise_weight = Eigen::MatrixXd::Ones(img.rows,img.cols);
  else horizontal_pairwise_weight = given_horizontal_pairwise_weight;

  Eigen::MatrixXd vertical_pairwise_weight;
  if (vertical_pairwise_weight.size()==0) vertical_pairwise_weight = Eigen::MatrixXd::Ones(img.rows,img.cols);
  else vertical_pairwise_weight = given_vertical_pairwise_weight;

  set_1st_order(img, img_filename, data_param, unary_weight, &gm);
  set_2nd_order(img, n_label, energy_param, horizontal_pairwise_weight, vertical_pairwise_weight, &gm);
  set_hamming_loss(img_filename, data_param, used_as_loss_augmented_inference, &gm);

  Eigen::MatrixXi ann(img.rows, img.cols);
  const string method = "ICM";//: "AlphaExpansion", "ICM"
  infer(method, gm, n_var, &ann);

  // debug_out_msg("shotton::annotate");
  return ann;
}

void set_hamming_loss(const std::string& id, DataParam data_param, const bool& active, 
                      GraphicalModel* gm) {
  // get GT annotation
  const std::string gt_ann_filepath = std::string(data_param["gt_csv_dir"]+"/"+id+".csv");

  Eigen::MatrixXi gt_ann;
  gt_ann = util::csv_read<Eigen::MatrixXi>(gt_ann_filepath);

  const int void_label = boost::lexical_cast<int>(data_param["void_label"]);

  // add factors
  const size_t n_col = gt_ann.cols();
  const size_t n_row = gt_ann.rows();
  const size_t y_size = gt_ann.size();

  for (size_t x=0; x<n_col; ++x) {
    for (size_t y=0; y<n_row; ++y) {
      HammingLossFunctor<float> f(active, gt_ann(y,x), void_label, y_size);
      GraphicalModel::FunctionIdentifier fid = gm->addFunction(f);

      size_t flat_idxes[] = {util::flat_idx_xy(x,y,n_col), util::flat_idx_xy(x+1,y,n_col)};
      std::sort(flat_idxes, flat_idxes + 1);
      gm->addFactor(fid, flat_idxes, flat_idxes + 1);
    }
  }
}

// void set_zero_one_loss(const size_t& n_row, const size_t& n_col, const bool& active, GraphicalModel* gm) {
//   // add factors
//   for (size_t x=0; x<n_col; ++x) {
//     for (size_t y=0; y<n_row; ++y) {
//       bool last_node = false;
//       if ( (x==(n_col-1)) and (y==(n_row-1)) )
//         last_node = true;

//       ZeroOneLossFunctor<float> f(last_node, active);
//       GraphicalModel::FunctionIdentifier fid = gm->addFunction(f);

//       size_t flat_idxes[] = {util::flat_idx_xy(x,y,n_col), util::flat_idx_xy(x+1,y,n_col)};
//       std::sort(flat_idxes, flat_idxes + 1);
//       gm->addFactor(fid, flat_idxes, flat_idxes + 1);
//     }
//   }
// } 

void set_1st_order(const cv::Mat& img, const std::string& img_filename, DataParam data_param, 
                   const Eigen::MatrixXd& weight, 
                   GraphicalModel* gm) {
  using namespace std;

  const string unary_prob_img_dir = data_param["unary_potential_dir"];
  const size_t n_label = boost::lexical_cast<size_t>(data_param["n_label"]);

  const string unary_prob_img_path = string( unary_prob_img_dir+"/"+img_filename+".unary" );
  // cout << "unary_prob_img_path= " << unary_prob_img_path << endl;

  ProbImage unary_prob_img;
  unary_prob_img.load( unary_prob_img_path.c_str() );

  assert(unary_prob_img.width()==img.cols && "unary_prob_img.width()!=img.cols");
  assert(unary_prob_img.height()==img.rows && "unary_prob_img.width()!=img.rows");

  for (size_t x=0; x<img.cols; ++x) {
    for (size_t y=0; y<img.rows; ++y) {
      // add a function
      const size_t shape[] = {n_label};
      opengm::ExplicitFunction<float> energy(shape, shape+1);

      for(int i = 0; i < n_label; i++) {
        energy(i) = philipp_unary_mrf::get_unary_potential_from_unary_prob_image(unary_prob_img,x,y,i);
        energy(i) = energy(i) * weight(y,x);
      } 

      GraphicalModel::FunctionIdentifier fid = gm->addFunction(energy);
      
      // add a factor
      size_t flat_idxes[] = {util::flat_idx_xy(x, y, img.cols)};
      gm->addFactor(fid, flat_idxes, flat_idxes+1);
    }
  }
}

void set_2nd_order(const cv::Mat& img, const size_t& n_label, EnergyParam energy_param, 
                   const Eigen::MatrixXd& horizontal_weight, const Eigen::MatrixXd& vertical_weight, 
                   GraphicalModel* gm) {
  // Params needed by the Pott model
  const float equal_pen = 0.0;

  //
  float beta;
  beta = edge_potential::get_beta(img);

  Eigen::MatrixXd theta_phi(2, 1);
  theta_phi << energy_param["theta_phi_1"], 
               energy_param["theta_phi_2"];

  //
  for (size_t x=0; x<img.cols; ++x) {
    for (size_t y=0; y<img.rows; ++y) {
      cv::Point2i p1;   
      p1.x = x; p1.y = y;

      // horizontal (to the right) connection:
      // (x, y) -- (x + 1, y) 
      if (x+1 < img.cols) {
        // add a function
        cv::Point2i p2;   
        p2.x = x+1; p2.y = y;

        float unequal_pen;
        unequal_pen = edge_potential::potential(img.at<cv::Vec3b>(p1), img.at<cv::Vec3b>(p2), beta, theta_phi);
        unequal_pen = unequal_pen * horizontal_weight(y,x);

        //
        opengm::PottsFunction<float> pott(n_label, n_label, equal_pen, unequal_pen);
        GraphicalModel::FunctionIdentifier fid = gm->addFunction(pott);

        // add a factor
        size_t flat_idxes[] = {util::flat_idx_xy(x,y,img.cols), util::flat_idx_xy(x+1,y,img.cols)};
        std::sort(flat_idxes, flat_idxes + 2);
        gm->addFactor(fid, flat_idxes, flat_idxes + 2);
      }

      // vertical (to the bottom (down)) connection:
      // (x, y) -- (x, y + 1)
      if (y+1 < img.rows) {
        // add a function
        cv::Point2i p2;   
        p2.x = x; p2.y = y+1;

        float unequal_pen;
        unequal_pen = edge_potential::potential(img.at<cv::Vec3b>(p1), img.at<cv::Vec3b>(p2), beta, theta_phi);
        unequal_pen = unequal_pen * vertical_weight(y,x);

        //
        opengm::PottsFunction<float> pott(n_label, n_label, equal_pen, unequal_pen);
        GraphicalModel::FunctionIdentifier fid = gm->addFunction(pott);

        // add a factor
        size_t flat_idxes[] = {util::flat_idx_xy(x,y,img.cols), util::flat_idx_xy(x,y+1,img.cols)};
        std::sort(flat_idxes, flat_idxes + 2);
        gm->addFactor(fid, flat_idxes, flat_idxes + 2);
      }
    }
  }
}

void infer(const std::string& method, const GraphicalModel& gm, const size_t& n_var, Eigen::MatrixXi* ann) {
  using namespace std;
  // cout << "infer(): BEGIN\n";

  vector<size_t> ann_vec(n_var);

  cout << method << " is inferring ..." << endl;
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

  // std::cout << "infer(): END\n";
}

}// namespace shotton
} // namespace lab1231_sun_prj
