#include <util/util.h>

namespace lab1231_sun_prj {
namespace util {

size_t n_pairwise(const size_t& n_row, const size_t& n_col, const std::string& type) {
  if (type=="N4") {
    return 2 * n_row * n_col - (n_row + n_col);
  }
  else {
    assert(false && "UNKNOWN TYPE");
  }
}

size_t flat_idx(const size_t& x, const size_t& y, const size_t& n_col) {
  return x + n_col * y;
}

size_t get_row(size_t index, size_t n_col) {
  return index / n_col;
}

size_t get_col(size_t index, size_t n_col) {
  return index % n_col;
}

double l1_norm(cv::Vec3b p1, cv::Vec3b p2) {
  double res = 0.0;
  for (int i=0; i < 3; i++){
    res += abs(p1[i] - p2[i]);
  }
  return res;
}

double l1_norm_squared(cv::Vec3b p1, cv::Vec3b p2) {
  return pow(l1_norm(p1,p2), 2);
}

std::vector<std::string> read_list(const std::string& list_path) {
  using namespace std;
  
  vector<string> test_img_filenames;

  ifstream test_img_list_file(list_path.c_str());
  if (test_img_list_file.is_open()) {
    string line;
    while ( getline(test_img_list_file,line) ) {
      test_img_filenames.push_back(line);
    }
    test_img_list_file.close();
  }
  else {
    cerr << "test_img_list_file.is_open(): FALSE\n";
  }

  return test_img_filenames;
}

Eigen::MatrixXi arr2mat(int* arr, const size_t& n_row, const size_t& n_col) {
  Eigen::MatrixXi mat(n_row, n_col);

  size_t idx = 0;
  for (size_t i=0; i<n_row; ++i)
    for (size_t j=0; j<n_col; ++j) {
      mat(i,j) = arr[idx];
      ++idx;
    }

  return mat;
}

cv::Mat ann2img(const Eigen::MatrixXi& ann, const std::string& dataset) {
  cv::Mat img(ann.rows(), ann.cols(), CV_8UC3, cv::Scalar(0,0,0));

  color_map::ClassColorMap map;
  map = color_map::class_color_map(dataset);

  for (size_t x=0; x<ann.cols(); ++x) 
    for (size_t y=0; y<ann.rows(); ++y) 
      img.at<cv::Vec3b>(cv::Point(x,y)) = map[ann(y,x)];
  
  return img;
}

}// namespace util
} // namespace lab1231_sun_prj