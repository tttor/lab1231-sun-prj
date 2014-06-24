#ifndef LAB1231_SUN_PRJ_EIGENLIB_SUPPORT_H_
#define LAB1231_SUN_PRJ_EIGENLIB_SUPPORT_H_

#include <string>
#include <fstream>
#include <cstdint>
#include <iostream>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/tokenizer.hpp>
#include <eigen3/Eigen/Dense>

namespace lab1231_sun_prj {

class EigenlibSupport {
 public:
  static Eigen::MatrixXd remove_rows(const Eigen::MatrixXd& M, const uint64_t& pstart, const uint64_t& pend);
  static Eigen::MatrixXd remove_row(const Eigen::MatrixXd& M, const uint64_t& ith);
  static Eigen::MatrixXd shift_mat(Eigen::MatrixXd mat);
  
  static double round_float(const double& val, const uint8_t& n_float_digit, const std::string type="nearest");
  static Eigen::MatrixXd round_float(const Eigen::MatrixXd& mat, const uint8_t& n_float_digit, const std::string type="nearest");
  static Eigen::MatrixXd truncate(const Eigen::MatrixXd& mat_in, const uint8_t& n_float_digit);
  
  static std::vector< std::vector<double> > mat2stdvec(const Eigen::MatrixXd& mat);
  static Eigen::MatrixXd stdvec2mat(const std::vector< std::vector<double> >& vec);
  static Eigen::MatrixXd scalar2mat(const double& scalar);
  static Eigen::MatrixXd rowvec2mat(const Eigen::RowVectorXd& vec);
  static double mat2scalar(const Eigen::MatrixXd& mat);
  
  static Eigen::MatrixXd load(std::string csv_path);
  static void write(Eigen::MatrixXd, std::string csv_path);
  static void write(Eigen::VectorXd vec, std::string csv_path);
};

}// namespace lab1231_ecg_prj

#endif



