#ifndef LAB1231_SUN_PRJ_SHOTTON_H_
#define LAB1231_SUN_PRJ_SHOTTON_H_

#include <iostream>
#include <map>
#include <algorithm> // std::sort
#include <boost/lexical_cast.hpp>
#include <Eigen/Dense>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <opengm/graphicalmodel/graphicalmodel.hxx>
#include <opengm/graphicalmodel/space/simplediscretespace.hxx>
#include <opengm/functions/potts.hxx>
#include <opengm/operations/adder.hxx>
#include <opengm/inference/messagepassing/messagepassing.hxx>
#include <opengm/inference/gibbs.hxx>
#include <opengm/inference/icm.hxx>
#include <opengm/inference/lazyflipper.hxx>
#include <opengm/inference/graphcut.hxx>
#include <opengm/inference/auxiliary/minstcutkolmogorov.hxx>
#include <opengm/inference/alphaexpansion.hxx>
#include <opengm/inference/alphaexpansionfusion.hxx>
#include <opengm/opengm.hxx>
#include <opengm/datastructures/marray/marray.hxx>
#include <opengm/graphicalmodel/space/simplediscretespace.hxx>
#include <opengm/graphicalmodel/graphicalmodel.hxx>
#include <opengm/operations/adder.hxx>
#include <opengm/functions/absolute_difference.hxx>
#include <opengm/operations/minimizer.hxx>

#include <prob-image/probimage.h>
#include <util/util.h>
#include <shotton/edge_potential.h>
#include <shotton/loss_functor.h>

namespace lab1231_sun_prj {
namespace shotton {

// typedef 
// opengm::GraphicalModel<
//   float,
//   opengm::Adder,
//   OPENGM_TYPELIST_2(opengm::ExplicitFunction<float>, opengm::PottsFunction<float>),
//   opengm::SimpleDiscreteSpace<size_t, size_t> > 
// GraphicalModel;

typedef 
opengm::GraphicalModel<
  float,
  opengm::Adder,
  OPENGM_TYPELIST_3(opengm::ExplicitFunction<float>, ZeroOneLossFunctor<float>,opengm::PottsFunction<float>),
  opengm::SimpleDiscreteSpace<size_t, size_t> > 
GraphicalModel;

typedef 
std::map<std::string, double> 
EnergyParam;

typedef 
std::map<std::string, std::string> 
DataParam;

/*!
 * @brief a dummy training: merely hardcode some given params in [Shotton, 2009] and/or [Kohli, 2009]
 */
void train(DataParam data_param, EnergyParam* energy_param);

/*!
 * @brief Annotate an image with given data_param, energy_param
 */
Eigen::MatrixXi annotate(const std::string& img_filename, DataParam data_param, EnergyParam energy_param, 
                         const Eigen::MatrixXd& unary_weight, 
                         const Eigen::MatrixXd& given_horizontal_pairwise_weight, 
                         const Eigen::MatrixXd& given_vertical_pairwise_weight,
                         const bool& used_as_loss_augmented_inference=false);

/*!
 * @brief we use Shotton's unary based on Phillipp's implementation
 */
void set_1st_order(const cv::Mat& img, const std::string& img_filename, DataParam data_param, 
                   const Eigen::MatrixXd& weight, 
                   GraphicalModel* gm);

/*!
 * @brief Use the edge potential of [Shotton, 2009], which follows the Pott model
 */
void set_2nd_order(const cv::Mat& img, const size_t& n_label, EnergyParam energy_param, 
                   const Eigen::MatrixXd& horizontal_weight, const Eigen::MatrixXd& vertical_weight,
                   GraphicalModel* gm);

/*!
 * @brief Accomodated method: AlphaExpansion, ICM, ...
 */
void infer(const std::string& method, const GraphicalModel& gm, const size_t& n_var, Eigen::MatrixXi* ann);

/*!
 * @brief Used for loss augmented inference
 */
void set_zero_one_loss(const size_t& n_row, const size_t& n_col, const bool& active, GraphicalModel* gm);

}// namespace shotton
} // namespace lab1231_sun_prj

#endif
