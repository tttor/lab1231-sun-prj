#ifndef LAB1231_SUN_PRJ_LADICKY_
#define LAB1231_SUN_PRJ_LADICKY_

#include <Eigen/Dense>
#include <boost/lexical_cast.hpp>

#include <robustpn/expand.h>
#include <prob-image/probimage.h>

#include <util/util.h>
#include <segmenter/segment_wrapper.h>
#include <shotton/edge_potential.h>
#include <ladicky/robustpn.h>

#include <opencv2/opencv.hpp>
#include <math.h>

namespace lab1231_sun_prj {
namespace ladicky {

/*!
 * @brief Annotate an image with given data_param, energy_param
 */
Eigen::MatrixXi annotate(const std::string& img_filename, const std::string& superpixel_filename, util::DataParam data_param, util::EnergyParam energy_param);

void set_1st_order(const size_t& n_row, const size_t& n_col, const size_t& n_label, const std::string& img_filename, util::DataParam data_param, Energy<double>* energy);

void set_2nd_order(const cv::Mat& img, util::EnergyParam energy_param, Energy<double>* energy);

void set_high_order(const cv::Mat& img, std::vector<util::Superpixel> superpixels, int n_label, const std::string& prob_img_filepath, Energy<double>* energy);

void infer(const std::string& method, Energy<double>* energy, Eigen::MatrixXi* ann);

}// namespace ladicky
}// namespace lab1231_sun_prj

#endif
