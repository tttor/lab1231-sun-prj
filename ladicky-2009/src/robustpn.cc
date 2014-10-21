#include <ladicky/robustpn.h>

namespace sun = lab1231_sun_prj;

double sun::ladicky::robustpn::gamma(cv::Mat img_rgb, sun::util::Superpixel superpixel, double theta_alpha, double theta_h_p, double theta_h_v, double theta_h_beta) {
  int num_pix = superpixel.size();
  double func_g = func_G(img_rgb, superpixel, theta_h_beta);

  double result = pow(num_pix, theta_alpha) * (theta_h_p + theta_h_v*func_g);
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

cv::Vec3f sun::ladicky::robustpn::get_mean(cv::Mat img_rgb, sun::util::Superpixel superpixel) {
  int c = superpixel.size();
  int n_cols = img_rgb.cols;

  cv::Vec3f result;
  result[0] = 0.0, result[1] = 0.0, result[2] = 0.0;
  
  for(int i = 0; i < c; i++){
    int row = sun::util::get_row(superpixel[i], n_cols), col = sun::util::get_col(superpixel[i], n_cols);
    result += img_rgb.at<cv::Vec3b>(row, col);
  }

  result[0] = result[0] / c; 
  result[1] = result[1] / c; 
  result[2] = result[2] / c;
  
  return result;
}