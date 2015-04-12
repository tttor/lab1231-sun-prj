#include <mrf_with_perf_pred/mrf_with_perf_pred.h>

namespace sun = lab1231_sun_prj;

Eigen::MatrixXi sun::mrf_with_perf_pred::annotate( const std::string& img_id, 
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
  superpixels = sun::util::load_sup2(segmentation_filepath);

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
  sun::mrf_with_perf_pred::set_1st_order(img, n_label, unary_philipp_filepath, energy);
  sun::mrf_with_perf_pred::set_2nd_order(img, energy_param, energy);
  sun::mrf_with_perf_pred::set_highest_order(img_id, superpixels, energy);

  //
  Eigen::MatrixXi ann(img.rows, img.cols);
  const string method = "AlphaExpansion";
  sun::mrf_with_perf_pred::infer(method, energy, &ann);
  delete energy;  

  #ifdef DEBUG_LEVEL_1
  cout << "annotate(): END\n";
  #endif
  return ann;
}

void sun::mrf_with_perf_pred::set_highest_order( const std::string& img_id, 
                                                 std::vector<sun::util::Superpixel> superpixels, 
                                                 Energy<double>* energy) {
using namespace std;
  #ifdef DEBUG_LEVEL_1
  cout << "set_highest_order(): BEGIN\n";
  #endif

  assert (superpixels.size()==1 && "FATAL: superpixels.size()!=1");

  //initialize number of elements in each segment
  energy->higherElements[0] = superpixels[0].size();

  //allocate energy for higher order indexes
  energy->AllocateHigherIndexes();
  for(int j = 0; j < superpixels[0].size(); j++){
    energy->higherIndex[0][j] = superpixels[0][j];
  }

  // initialize highest order energy
  // Note that we adopt the robust-pn higher order energy data-structure,
  // where there are gamma_k and gamma_max and truncation_ratio
  // here (in the ladicky-mrf (for mrf with perf pred)), we void _all_ of them, 
  // but initialized here for completeness
  size_t i = 0;// idx of the highest-order clique

  //truncation ratio 
  const double truncation_ratio = 1.0;
  energy->higherTruncation[i] = truncation_ratio * (energy->higherElements[i]);

  //gamma_k
  for(int k = 0; k < energy->nlabel; k++){
    const size_t gamma_k_idx = i * (energy->nlabel + 1) + k;
    const double gamma_k = 0.0;

    energy->higherCost[gamma_k_idx] = gamma_k;
  }

  //gamma_max
  const size_t gamma_max_idx = i * (energy->nlabel + 1) + energy->nlabel;
  const double gamma_max = 0.0;
  energy->higherCost[gamma_max_idx] = gamma_max;

  #ifdef DEBUG_LEVEL_1
  cout << "set_highest_order(): END\n";
  #endif
}

void sun::mrf_with_perf_pred::set_2nd_order(const cv::Mat& img, 
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

void sun::mrf_with_perf_pred::set_1st_order(const cv::Mat& img, 
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

void sun::mrf_with_perf_pred::infer(const std::string& method, 
                         Energy<double>* energy, Eigen::MatrixXi* ann) {
  using namespace std;
  #ifdef DEBUG_LEVEL_1
  cout << "infer(): BEGIN\n";
  #endif

  const size_t n_var = ann->rows() * ann->cols();

  //initialize solution
  cout << "initialize solution\n";
  int *ann_arr = new int[n_var];
  memset(ann_arr, 0, n_var * sizeof(int));

  //initialize alpha expansion
  cout << "initialize alpha expansion\n";
  const size_t max_iter = 10;
  AExpand *expand = new AExpand(energy, max_iter);

  //solve CRF
  cout << "solve CRF\n";
  expand->minimize(ann_arr);

  //
  *ann = sun::util::arr2mat(ann_arr, ann->rows(), ann->cols());
  delete expand;

  #ifdef DEBUG_LEVEL_1
  cout << "infer(): END\n";
  #endif
}
