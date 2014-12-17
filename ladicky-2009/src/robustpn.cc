#include <ladicky/robustpn.h>

namespace sun = lab1231_sun_prj;

double sun::ladicky::robustpn::gamma(cv::Mat img_rgb, sun::util::Superpixel superpixel, double theta_alpha, double theta_h_p, double theta_h_v, double theta_h_beta) {
  int num_pix = superpixel.size();
  double func_g = func_G(img_rgb, superpixel, theta_h_beta);

  double result = pow(num_pix, theta_alpha) * (theta_h_p + theta_h_v*func_g);
  return result;
}

double sun::ladicky::robustpn::gamma_unary(const std::string unary_img_path, cv::Mat img_rgb, int n_label, util::Superpixel superpixel, double theta_alpha, double theta_h_p, double theta_h_v, double theta_h_beta){
  int n_superpixel = superpixel.size();
  double func_g_unary = func_G_unary(unary_img_path, img_rgb, superpixel, theta_h_beta, n_label);
  double result = pow(n_superpixel, theta_alpha) * (theta_h_p + theta_h_v * func_g_unary);
  return result;
}

double sun::ladicky::robustpn::func_G(cv::Mat img_rgb, sun::util::Superpixel superpixel, double theta_h_beta) {
  int c = superpixel.size();
  int n_cols = img_rgb.cols;

  cv::Vec3f mean_color = get_mean(img_rgb, superpixel);
  
  cv::Vec2f result;
    result[0] = 0.0, result[1] = 0.0, result[2] = 0.0;

  for(int i = 0; i < c; i++){
    int row = sun::util::get_row(superpixel[i], n_cols), col = sun::util::get_col(superpixel[i], n_cols);
    cv::Vec3f color = img_rgb.at<cv::Vec3b>(row, col);

    result[0] += pow(color[0] - mean_color[0], 2);
    result[1] += pow(color[1] - mean_color[1], 2);
    result[2] += pow(color[2] - mean_color[2], 2);
  } 

  double norm_color = result[0] + result[1] + result[2];
  // debug("sum_feature",sum_feature);
  double res = -1 * theta_h_beta * ( norm_color / c );
  return exp(res);
}

double sun::ladicky::robustpn::func_G_unary(const std::string unary_img_path, cv::Mat img_rgb, util::Superpixel superpixel, double theta_h_beta, int n_label){
  int c = superpixel.size();
  int n_cols = img_rgb.cols;

  ProbImage prob_img;
  prob_img.decompress(unary_img_path.c_str());

  std::vector<double> mean_unary = get_mean_unary(unary_img_path, img_rgb, superpixel,n_label);

  std::vector<double> vector_result;
  for (int j = 0; j < n_label; j++){      
      vector_result.push_back(0.0);
  }

  for (int i = 0; i < c; i++){
    int row = sun::util::get_row(superpixel[i], n_cols), col = sun::util::get_col(superpixel[i], n_cols);
    for(int j = 0; j < n_label; j++){
      double energy_ = -1 * log(prob_img(col, row, j)+0.0000000001);
      vector_result.at(j) += pow(mean_unary[j] - energy_, 2);
    }
  }

  double norm_unary = 0.0;
  for (int j = 0; j < n_label; j++) 
    norm_unary += vector_result.at(j);

  // std::cout << "norm_unary " << norm_unary << std::endl;
  double res = -1 * theta_h_beta * ( norm_unary / c );
  // std::cout << "res " << exp(res) << std::endl;
  return exp(res);
}

cv::Vec3f sun::ladicky::robustpn::get_mean(cv::Mat img_rgb, sun::util::Superpixel superpixel) {
  int c = superpixel.size();
  int n_col = img_rgb.cols;

  cv::Vec3f result;
  result[0] = 0.0, result[1] = 0.0, result[2] = 0.0;
  
  for(int i = 0; i < c; i++){
    int row = sun::util::get_row(superpixel[i], n_col), col = sun::util::get_col(superpixel[i], n_col);
    result += img_rgb.at<cv::Vec3b>(row, col);
  }

  result[0] = result[0] / c; 
  result[1] = result[1] / c; 
  result[2] = result[2] / c;
  
  return result;
}

std::vector<double> sun::ladicky::robustpn::get_mean_unary(const std::string unary_img_path, cv::Mat img_rgb, util::Superpixel superpixel, int n_label){  
  int n_superpixel = superpixel.size();
  int n_col = img_rgb.cols;
  // int n_label = data_param["n_label"];

  ProbImage prob_img;
  prob_img.decompress(unary_img_path.c_str());
  // std::cout << unary_img_path << std::endl;

  std::vector<double> vector_unary;
  for (int j = 0; j < n_label; j++){      
      vector_unary.push_back(0.0);
  }
  
  // std::cout << n_superpixel << std::endl;  
  for (int i = 0; i < n_superpixel; i++){
    int row = sun::util::get_row(superpixel[i], n_col), col = sun::util::get_col(superpixel[i], n_col);
    for (int j = 0; j < n_label; j++){      
      vector_unary.at(j) += -1 * log(prob_img(col,row,j)+0.0000000001);
    }
  }

  for (int j = 0; j < n_label; j++){      
      vector_unary.at(j) /= (1.0 * n_superpixel);
      // std::cout << vector_unary.at(j) << std::endl;    
  }
  // std::cout << vector_unary << std::endl;
  return vector_unary;
}