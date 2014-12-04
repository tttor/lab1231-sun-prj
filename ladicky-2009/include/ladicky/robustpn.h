#ifndef LAB1231_SUN_PRJ_ROBUSTPN_
#define LAB1231_SUN_PRJ_ROBUSTPN_

#include <opencv2/opencv.hpp>
#include <math.h>
#include <string>

#include <util/util.h>
#include <prob-image/probimage.h>

namespace lab1231_sun_prj {
namespace ladicky {
namespace robustpn {

cv::Vec3f get_mean(cv::Mat img_rgb, util::Superpixel superpixel);
std::vector<double> get_mean_unary(const std::string unary_img_path, cv::Mat img_rgb, util::Superpixel superpixel, int n_label);

double func_G(cv::Mat img_rgb, util::Superpixel superpixel, double theta_h_beta);
double func_G_unary(const std::string unary_img_path, cv::Mat img_rgb, util::Superpixel superpixel, double theta_h_beta, int n_label);

double gamma(cv::Mat img_rgb, util::Superpixel superpixel, double theta_alpha, double theta_h_p, double theta_h_v, double theta_h_beta);
double gamma_unary(const std::string unary_img_path, cv::Mat img_rgb, int n_label, util::Superpixel superpixel, double theta_alpha, double theta_h_p, double theta_h_v, double theta_h_beta);

}// namespace robustpn
}// namespace ladicky
}// namespace lab1231_sun_prj
#endif
