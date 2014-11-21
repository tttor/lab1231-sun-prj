#ifndef PHILIPP_UNARY_MRF_H_
#define PHILIPP_UNARY_MRF_H_

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
#include <opencv2/opencv.hpp>
#include <util/util.h>
#include <prob-image/probimage.h>

namespace sun = lab1231_sun_prj;

typedef 
opengm::GraphicalModel<
  double,
  opengm::Adder,
  OPENGM_TYPELIST_1(opengm::ExplicitFunction<double>),
  opengm::SimpleDiscreteSpace<size_t, size_t> > 
GraphicalModel;

Eigen::MatrixXi annotate(const std::string& img_filename, sun::util::DataParam data_param);
void set_1st_order(const std::string& img_filename, const size_t& n_label, const std::string& unary_prob_img_dir, GraphicalModel* gm);
void infer(const std::string& method, const GraphicalModel& gm, const size_t& n_var, Eigen::MatrixXi* ann);

#endif