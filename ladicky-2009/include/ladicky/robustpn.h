#ifndef LAB1231_SUN_PRJ_ROBUSTPN_
#define LAB1231_SUN_PRJ_ROBUSTPN_

#include <opencv2/opencv.hpp>
#include <math.h>

#include <util/util.h>

namespace lab1231_sun_prj {
namespace ladicky {
namespace robustpn {

cv::Vec3f get_mean(cv::Mat img_rgb, util::Superpixel superpixel);

double func_G(cv::Mat img_rgb, util::Superpixel superpixel, double theta_h_beta);

double gamma(cv::Mat img_rgb, util::Superpixel superpixel, double theta_alpha, double theta_h_p, double theta_h_v, double theta_h_beta);

}// namespace robustpn
}// namespace ladicky
}// namespace lab1231_sun_prj
#endif
