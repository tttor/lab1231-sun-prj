#ifndef LAB1231_SUN_PRJ_SHOTTON_EDGE_POTENTIAL_H_
#define LAB1231_SUN_PRJ_SHOTTON_EDGE_POTENTIAL_H_

#include <Eigen/Dense>
#include <opencv2/core/core.hpp>
#include <util/util.h>

namespace lab1231_sun_prj {
namespace shotton {
namespace edge_potential {

/*!
 * @brief
 */
float get_beta(cv::Mat &image_rgb);
float get_beta(cv::Mat *image_rgb);

/*!
 * @brief
 */
double potential(const cv::Vec3b& x1, const cv::Vec3b& xj, const double& beta, const Eigen::MatrixXd& theta_phi);


}// namespace edge_potential
}// namespace shotton
}// namespace lab1231_sun_prj

#endif