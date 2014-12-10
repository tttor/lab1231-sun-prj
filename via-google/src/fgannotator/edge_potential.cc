#include <shotton/edge_potential.h>

namespace sun = lab1231_sun_prj;

float sun::shotton::edge_potential::get_beta(cv::Mat &image_rgb){
  float total = 0;
  int n_total = 0;
  
  for (size_t x=0; x<image_rgb.cols; ++x) {
    for (size_t y=0; y<image_rgb.rows; ++y) {      
        if (x+1 < image_rgb.cols){
          cv::Vec3b p1 = image_rgb.at<cv::Vec3b>(cv::Point2i(x,y));
          cv::Vec3b p2 = image_rgb.at<cv::Vec3b>(cv::Point2i(x+1,y));       
          total += sun::util::l1_norm_squared(p1,p2);
          ++n_total;
        }
        if (y+1 < image_rgb.rows){
          cv::Vec3b p1 = image_rgb.at<cv::Vec3b>(cv::Point2i(x,y));
          cv::Vec3b p2 = image_rgb.at<cv::Vec3b>(cv::Point2i(x,y+1));       
          total += sun::util::l1_norm_squared(p1,p2);
          ++n_total;
        }      
    }
  }

  float average = total / (float) n_total;
  float result = 1.0 / ( 2.0 * average);
  return result;
}

double sun::shotton::edge_potential::potential(const cv::Vec3b& xi, const cv::Vec3b& xj, const double& beta, const Eigen::MatrixXd& theta_phi) {
  // Eq.8 of [Shotton, 2009]
  Eigen::MatrixXd g_ij(2, 1);
  g_ij << exp( -1 * beta * sun::util::l1_norm_squared(xi,xj) ),
                                1.0                           ;

  // Eq.7 of [Shotton, 2009]
  Eigen::MatrixXd potential;
  potential = theta_phi.transpose().conjugate() * g_ij;

  return potential(0);
}