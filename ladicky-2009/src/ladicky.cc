#include <ladicky/ladicky.h>

namespace sun = lab1231_sun_prj;

Eigen::MatrixXi sun::ladicky::annotate(const std::string& img_id, 
                                       sun::util::DataParam data_param, 
                                       sun::util::EnergyParam energy_param) {
  using namespace std;
  #ifdef DEBUG_LEVEL_1
  cout << "annotate(): BEGIN\n";
  #endif

  const string img_filepath = string(data_param["img_dir"]+"/"+img_id+data_param["img_extension"]);
  cv::Mat img = cv::imread(img_filepath, CV_LOAD_IMAGE_COLOR);

  // Segmentation for hi-order energy
  vector<sun::util::Superpixel> superpixels;
  const string segmentation_filepath = string(data_param["segmentation_dir"]+"/"+img_id+"/"
                                              +img_id+data_param["segmentation_param"]+".sup2");
  cout << "segmentation_filepath= " << segmentation_filepath << endl;
  superpixels = sun::util::load_superpixel(segmentation_filepath);

  //inititialize energy 
  const size_t n_var = img.rows * img.cols;
  const size_t n_label = boost::lexical_cast<size_t>(data_param["n_label"]);
  const size_t n_segment = superpixels.size();
  const size_t n_pairwise = sun::util::n_pairwise(img.rows, img.cols,"N4");
  const std::string unary_philipp_filepath = std::string(data_param["unary_philipp_dir"]+"/"
                                                         +img_id+".c_unary");

  Energy<double>* energy;
  energy = new Energy<double>(n_label, n_var, n_pairwise, n_segment);

  //
  sun::ladicky::set_1st_order(img, n_label, unary_philipp_filepath, energy);
  sun::ladicky::set_2nd_order(img, energy_param, energy);
  if (data_param["segmentation_param"]=="-onesuperpixel") 
    sun::ladicky::set_highest_order(img_id, superpixels, energy);
  else
    sun::ladicky::set_high_order(img, superpixels, unary_philipp_filepath, n_label, energy);

  //
  Eigen::MatrixXi ann(img.rows, img.cols);
  const string method = "AlphaExpansion";
  sun::ladicky::infer(method, energy, &ann);

  delete energy;  

  #ifdef DEBUG_LEVEL_1
  cout << "annotate(): END\n";
  #endif
  return ann;
}

void sun::ladicky::infer(const std::string& method, 
                         Energy<double>* energy, Eigen::MatrixXi* ann) {
  using namespace std;
  #ifdef DEBUG_LEVEL_1
  cout << "infer(): BEGIN\n";
  #endif

  const size_t n_var = ann->rows() * ann->cols();

  //initialize solution
  int *ann_arr = new int[n_var];
  memset(ann_arr, 0, n_var * sizeof(int));

  //initialize alpha expansion
  const size_t max_iter = 10;
  AExpand *expand = new AExpand(energy, max_iter);

  //solve CRF
  expand->minimize(ann_arr);

  //
  *ann = sun::util::arr2mat(ann_arr, ann->rows(), ann->cols());
  delete expand;

  #ifdef DEBUG_LEVEL_1
  cout << "infer(): END\n";
  #endif
}

double sun::ladicky::get_predicted_perf_ca(const std::string& img_id) {
  return 1.0;
}

void sun::ladicky::set_highest_order(const std::string& img_id, 
                                     std::vector<sun::util::Superpixel> superpixels, 
                                     Energy<double>* energy) {
  using namespace std;
  #ifdef DEBUG_LEVEL_1
  cout << "set_highest_order(): BEGIN\n";
  #endif

  //initialize number of elements in each segment
  cout << "superpixels.size()= " << superpixels.size() << endl;
  energy->higherElements[0] = superpixels[0].size();

  //allocate energy for higher order indexes
  energy->AllocateHigherIndexes();
  energy->higherIndex[0][0] = superpixels[0][0];

  // The Robust P n model potentials take the form:
  // gamma_kprime = min{ (|c|-n_k(x_c))*theta_k +gamma_k }
  // gamma_c(x_c) = min{gamma_kprime, gamma_max}
  // (17)
  const size_t perf_ca_max = 1.0;
  
  //initialize truncation ratio Q for each clique
  energy->higherTruncation[0] = 0.0;

  //initialize gamma_k for each clique
  for(int k = 0; k < energy->nlabel; k++) 
      energy->higherCost[k] = perf_ca_max;

  //initialize gamma_max for each clique
  const size_t higher_cost_idx = energy->nlabel;
  energy->higherCost[higher_cost_idx] = get_predicted_perf_ca(img_id);

  #ifdef DEBUG_LEVEL_1
  cout << "set_highest_order(): END\n";
  #endif
}

void sun::ladicky::set_high_order(const cv::Mat& img,
                                  const std::vector<sun::util::Superpixel>& superpixels,
                                  const std::string& unary_philipp_filepath,
                                  const size_t& n_label,
                                  Energy<double>* energy) {
  using namespace std;
  #ifdef DEBUG_LEVEL_1
  cout << "set_high_order(): BEGIN\n";
  #endif

  //initialize number of elements in each segment
  cout << "superpixels.size()= " << superpixels.size() << endl;
  for (int i = 0; i < superpixels.size(); i++) {
    cout << "superpixels[i].size()= " << superpixels[i].size() << endl;
    energy->higherElements[i] = superpixels[i].size();
  }

  //allocate energy for higher order indexes
  energy->AllocateHigherIndexes();
  for (int i = 0; i < superpixels.size(); i++) for(int j = 0; j < superpixels[i].size(); j++){
    energy->higherIndex[i][j] = superpixels[i][j];
  }

  //initialize truncation ratio Q, gamma_k and gamma_max for each clique
  // cout << "energy->nhigher=" << energy->nhigher << endl;
  for(int i = 0; i < energy->nhigher; i++)
  {
    //truncation ratio 30%
    energy->higherTruncation[i] = 0.3 * (energy->higherElements[i]);
    // cout << "n_segment" << (energy->higherElements[i]) << endl;
    // cout << "higher truncation" << energy->higherTruncation[i] << endl;

    //gamma_k
    for(int k = 0; k < energy->nlabel; k++) 
      energy->higherCost[i * (energy->nlabel + 1) + k] = 0;//get_gamma_k(superpixels[i], k);

    //gamma_max
    // energy->higherCost[i * (energy->nlabel + 1) + energy->nlabel] = sun::ladicky::robustpn::gamma(img, superpixels[i], 0.8, 0.2, 0.5, 12.0);
    energy->higherCost[i * (energy->nlabel + 1) + energy->nlabel] = 100 * sun::ladicky::robustpn::gamma_unary(unary_philipp_filepath, img, n_label, superpixels[i], 0.8, 0.2, 0.5, 12.0);
  }

  #ifdef DEBUG_LEVEL_1
  cout << "set_high_order(): END\n";
  #endif
}

void sun::ladicky::set_2nd_order(const cv::Mat& img, 
                                 sun::util::EnergyParam energy_param, 
                                 Energy<double>* energy) {
  using namespace std;
  #ifdef DEBUG_LEVEL_1
  cout << "set_2nd_order(): BEGIN\n";
  #endif

  int ind_2nd = 0, ind_2nd_energy = 0;
  double energy_res = 0.0;
  double beta = sun::shotton::edge_potential::get_beta(img);

  Eigen::MatrixXd theta_phi(2, 1);
  theta_phi << energy_param["theta_phi_1"], 
               energy_param["theta_phi_2"];

  for (int x = 0; x < img.cols; x++) for (int y=0; y<img.rows; y++) {
    cv::Point2i p1;   
    p1.x = x; p1.y = y;

    if (x+1 < img.cols) {// (x, y) -- (x + 1, y)
      cv::Point2i p2;   
      p2.x = x+1; p2.y = y;
      
      energy->pairIndex[ind_2nd++] = sun::util::flat_idx_xy(x, y, img.cols);
      energy->pairIndex[ind_2nd++] = sun::util::flat_idx_xy(x+1, y, img.cols);
      energy_res = sun::shotton::edge_potential::potential(img.at<cv::Vec3b>(p1), img.at<cv::Vec3b>(p2), beta, theta_phi);
      energy->pairCost[ind_2nd_energy++] =  energy_res;
    } 
    
    if (y+1 < img.rows) {// (x, y) -- (x, y + 1)
      cv::Point2i p2;   
      p2.x = x; p2.y = y+1;

      energy->pairIndex[ind_2nd++] = sun::util::flat_idx_xy(x, y, img.cols);
      energy->pairIndex[ind_2nd++] = sun::util::flat_idx_xy(x, y+1, img.cols);
      energy_res = sun::shotton::edge_potential::potential(img.at<cv::Vec3b>(p1), img.at<cv::Vec3b>(p2), beta, theta_phi);
      energy->pairCost[ind_2nd_energy++] =  energy_res;
    }
  }   

  #ifdef DEBUG_LEVEL_1
  cout << "set_2nd_order(): END\n";
  #endif
}

void sun::ladicky::set_1st_order(const cv::Mat& img, 
                                 const size_t& n_label, 
                                 const std::string& unary_philipp_filepath, 
                                 Energy<double>* energy) {
  using namespace std;
  #ifdef DEBUG_LEVEL_1
  cout << "set_1st_order(): BEGIN\n";
  #endif

  ProbImage prob_img;
  prob_img.decompress(unary_philipp_filepath.c_str());
  assert(prob_img.width()==img.cols && "prob_img.width()!=img.cols");
  assert(prob_img.height()==img.rows && "prob_img.height()!=img.rows");
  assert(prob_img.depth()==n_label && "prob_img.depth()!=n_label");

  const size_t n_col = img.cols;
  const size_t n_row = img.rows;
  for (int x = 0; x < n_col; x++) for (int y=0; y<n_row; y++){
    for(int j = 0; j < n_label; j++){
      int index = sun::util::flat_idx_xy(x, y, n_col);
      energy->unaryCost[index * energy->nlabel + j] =  -1 * log( prob_img(x,y,j) ); //-1 * prob_img(x,y,j);
    }
  }

  #ifdef DEBUG_LEVEL_1
  cout << "set_1st_order(): END\n";
  #endif
}