#ifndef LAB1231_SUN_PRJ_MAJORITY_H_
#define LAB1231_SUN_PRJ_MAJORITY_H_
#include <iostream>
#include <Eigen/Dense>

namespace lab1231_sun_prj {
namespace majority {

size_t*** init_buffer_result(int n_row, int n_col, int n_label);
size_t*** add_result_to_buffer(size_t*** buff, Eigen::MatrixXi& ann, int n_row, int n_col);
size_t get_max_label(size_t* arr, size_t n_label);
Eigen::MatrixXi get_final_result_from_buffer(size_t*** buff, int n_row, int n_col, int n_label);

} //majority namespace
} //majority lab1231

#endif