#include <majority/majority.h>

namespace lab1231_sun_prj {
namespace majority {

size_t*** init_buffer_result(int n_row, int n_col, int n_label){
  size_t*** array;    // 3D array definition;
  array = new size_t**[n_row];
  for(int x = 0; x < n_row; ++x) {
      array[x] = new size_t*[n_col];
      for(int y = 0; y < n_col; ++y) {
          array[x][y] = new size_t[n_label];
          for(int z = 0; z < n_label; ++z) { // initialize the values to whatever you want the default to be
              array[x][y][z] = 0;
          }
      }
  }
  return array;
}

 size_t*** add_result_to_buffer(size_t*** buff, Eigen::MatrixXi& ann, int n_row, int n_col){
  using namespace std;
  for(int i = 0; i < n_row; i++){
    for(int j = 0; j < n_col; j++){
      buff[i][j][ ann(i,j) ] += 1;
    }
  }
  return buff;
}

size_t get_max_label(size_t* arr, size_t n_label){
  size_t label_max = 0;
  size_t maxi = 0;
  for (size_t i = 0; i < n_label; i++){
    if (maxi < arr[i]){
      maxi = arr[i];
      label_max = i;
    }
  }
  return label_max;
}

Eigen::MatrixXi get_final_result_from_buffer(size_t*** buff, int n_row, int n_col, int n_label){
  Eigen::MatrixXi mat(n_row, n_col);

  for(int i = 0; i < n_row; i++){
    for(int j = 0; j < n_col; j++){      
      mat(i,j) = get_max_label(buff[i][j], n_label);
    }
  }
  return mat;
}

}
}