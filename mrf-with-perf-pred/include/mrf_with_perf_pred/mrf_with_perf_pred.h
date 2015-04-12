#ifndef LAB1231_SUN_PRJ_MRF_WITH_PERF_PRED_H
#define LAB1231_SUN_PRJ_MRF_WITH_PERF_PRED_H

#include <Eigen/Dense>
#include <boost/lexical_cast.hpp>
#include <opencv2/opencv.hpp>
#include <math.h>
#include <unistd.h>
#include <sys/time.h>

#include <util/util.h>
#include <shotton/edge_potential.h>
#include <ladicky-mrf/expand.h>
#include <prob-image/probimage.h>

namespace lab1231_sun_prj {
namespace mrf_with_perf_pred {

/*!
 * @brief Annotate an image with given data_param, energy_param
 */
Eigen::MatrixXi annotate(const std::string& img_filename, 
                         util::DataParam data_param, 
                         util::EnergyParam energy_param);

void infer(const std::string& method, 
           Energy<double>* energy, 
           Eigen::MatrixXi* ann);

void set_1st_order(const cv::Mat& img, 
                   const size_t& n_label, 
                   const std::string& unary_philipp_filepath, 
                   Energy<double>* energy);

void set_2nd_order(const cv::Mat& img, 
                   util::EnergyParam energy_param, 
                   Energy<double>* energy);

void set_highest_order(const std::string& img_id, 
                       std::vector<util::Superpixel> superpixels, 
                       Energy<double>* energy);

}// namespace mrf_with_perf_pred
}// namespace lab1231_sun_prj

#endif
