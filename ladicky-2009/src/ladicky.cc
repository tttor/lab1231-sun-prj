#include <ladicky/ladicky.h>

namespace sun = lab1231_sun_prj;

Eigen::MatrixXi sun::ladicky::annotate(const std::string& img_filename, sun::util::DataParam data_param, sun::util::EnergyParam energy_param) {
  using namespace std;
  cout << "annotate(): BEGIN\n";

  //
  const string img_path = string(data_param["ori_img_dir"]+img_filename);
  cv::Mat img = cv::imread(img_path, CV_LOAD_IMAGE_COLOR);

  // Segmentation for hi-order energy
  const size_t region = energy_param["SLIC_region"];
  const size_t regularization = energy_param["SLIC_regularization"];
  const size_t min_region = energy_param["SLIC_min_region"];

  vector<sun::util::Superpixel> superpixels;
  superpixels = sun::slic_segment_wrapper::segment(img, region, regularization, min_region);

  //inititialize energy 
  const size_t n_var = img.rows * img.cols;
  const size_t n_label = boost::lexical_cast<size_t>(data_param["n_label"]);
  const size_t n_segment = superpixels.size();
  const size_t n_pairwise = sun::util::n_pairwise(img.rows, img.cols,"N4");

  Energy<double>* energy;
  energy = new Energy<double>(n_label, n_var, n_pairwise, n_segment);

  //
  sun::ladicky::set_1st_order(img.rows, img.cols, n_label, img_filename, data_param, energy);
  sun::ladicky::set_2nd_order(img, energy_param, energy);
  sun::ladicky::set_high_order(img, superpixels, energy);

  //
  Eigen::MatrixXi ann(img.rows, img.cols);
  const string method = "AlphaExpansion";
  sun::ladicky::infer(method, energy, &ann);

  cout << "annotate(): END\n";
  return ann;
}

void sun::ladicky::infer(const std::string& method, Energy<double>* energy, Eigen::MatrixXi* ann) {
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
}

void sun::ladicky::set_high_order(const cv::Mat& img, std::vector<sun::util::Superpixel> superpixels, Energy<double>* energy) {
  using namespace std;

  //initialize number of elements in each segment
  int maxi = 0;
  for (int i = 0; i < superpixels.size(); i++) {
    energy->higherElements[i] = superpixels[i].size();

    if (maxi < superpixels[i].size())
      maxi = superpixels[i].size();
  }
  cout << "maxi " << maxi << endl;

  //allocate energy for higher order indexes
  energy->AllocateHigherIndexes();

  for (int i = 0; i < superpixels.size(); i++) for(int j = 0; j < superpixels[i].size(); j++){
    energy->higherIndex[i][j] = superpixels[i][j];
  }

  //initialize truncation ratio Q, gamma_k and gamma_max for each clique
  for(int i = 0; i < energy->nhigher; i++)
  {
    //truncation ratio 30%
    energy->higherTruncation[i] = 0.3;

    //gamma_k
    for(int k = 0; k < energy->nlabel; k++) 
      energy->higherCost[i * (energy->nlabel + 1) + k] = 0;//get_gamma_k(superpixels[i], k);

    //gamma_max
    energy->higherCost[i * (energy->nlabel + 1) + energy->nlabel] = sun::ladicky::robustpn::gamma(img, superpixels[i], 0.8, 0.2, 0.5, 12.0);
  }
}

void sun::ladicky::set_2nd_order(const cv::Mat& img, sun::util::EnergyParam energy_param, Energy<double>* energy) {
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
      
      energy->pairIndex[ind_2nd++] = sun::util::flat_idx(x, y, img.cols);
      energy->pairIndex[ind_2nd++] = sun::util::flat_idx(x+1, y, img.cols);
      energy_res = sun::shotton::edge_potential::potential(img.at<cv::Vec3b>(p1), img.at<cv::Vec3b>(p2), beta, theta_phi);
      energy->pairCost[ind_2nd_energy++] =  energy_res;
    } 
    
    if (y+1 < img.rows) {// (x, y) -- (x, y + 1)
      cv::Point2i p2;   
      p2.x = x; p2.y = y+1;

      energy->pairIndex[ind_2nd++] = sun::util::flat_idx(x, y, img.cols);
      energy->pairIndex[ind_2nd++] = sun::util::flat_idx(x, y+1, img.cols);
      energy_res = sun::shotton::edge_potential::potential(img.at<cv::Vec3b>(p1), img.at<cv::Vec3b>(p2), beta, theta_phi);
      energy->pairCost[ind_2nd_energy++] =  energy_res;
    }
  }   
}

void sun::ladicky::set_1st_order(const size_t& n_row, const size_t& n_col, const size_t& n_label, const std::string& img_filename, sun::util::DataParam data_param, Energy<double>* energy) {
  const std::string prob_img_filepath = std::string( data_param["unary_philipp_dir"]+img_filename.substr(0,img_filename.size()-4)+".c_unary" );// -4 for ".bmp"

  ProbImage prob_img;
  prob_img.decompress(prob_img_filepath.c_str());

  for (int x = 0; x < n_col; x++) for (int y=0; y<n_row; y++){
    for(int j = 0; j < n_label; j++){
      int index = sun::util::flat_idx(x, y, n_col);
      energy->unaryCost[index * energy->nlabel + j] = -1 * prob_img(x,y,j);
    }
  }
}