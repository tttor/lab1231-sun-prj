#ifndef UTIL_H
#define UTIL_H

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <fstream>

#include <boost/algorithm/string.hpp>

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
#include <opengm/opengm.hxx>
#include <opengm/datastructures/marray/marray.hxx>
#include <opengm/graphicalmodel/space/simplediscretespace.hxx>
#include <opengm/graphicalmodel/graphicalmodel.hxx>
#include <opengm/operations/adder.hxx>
#include <opengm/functions/absolute_difference.hxx>


#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std; // 'using' is used only in example code
using namespace opengm;

typedef std::map<std::string, double> LabColorSpace;

inline size_t var_idx(const size_t& x, const size_t& y, const size_t& n_col) {
  return x + n_col * y;
}

void compute_hist(const cv::Mat& src, const cv::Mat& mask, cv::Mat* hist) {
  using namespace cv;

  int histSize = 256;
  float range[] = { 0, 256 } ;
  const float* histRange = { range };
  bool uniform = true; 
  bool accumulate = false;

  calcHist( &src, 1, 0, mask, *hist, 1, &histSize, &histRange, uniform, accumulate );
}

void get_mask(const std::string& type, const std::string& label_path, cv::Mat* mask) {
  ifstream label_file(label_path.c_str());
  vector< vector<string> > label_str;
  
  if (label_file.is_open()) {
    string line;
    while ( getline(label_file,line) )
    {
      vector<string> label_chars;
      boost::split( label_chars, line, boost::algorithm::is_any_of(" "), boost::token_compress_on );
      assert(label_chars.size() == mask->cols && "label_chars.size() != mask->cols");

      label_str.push_back(label_chars);
    }
    label_file.close();
    assert(label_str.size() == mask->rows && "label_str.size() != mask->rows");
  }
  else {
    cout << "Unable to open file";
  } 

  //
  const string fore_label = "7";

  for (size_t x=0; x<mask->cols; ++x) {
    for (size_t y=0; y<mask->rows; ++y) {
      string label = label_str.at(y).at(x);

      if ( (type == "foreground" and label == fore_label)
          or (type == "background" and label != fore_label) ) {
        mask->at<unsigned char>(y,x,1) = 255;
      }
    }
  }
}

void draw_hist(const cv::Mat& hist) {
  using namespace cv;

  // Draw the histograms 
  int hist_w = 512; int hist_h = 400;
  int bin_w = cvRound( (double) hist_w/hist.rows );

  Mat histImage( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );

  /// Normalize the result to [ 0, histImage.rows ]
  normalize(hist, hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );

  for( int i = 1; i < hist.rows; i++ ) {
      line( histImage, Point( bin_w*(i-1), hist_h - cvRound(hist.at<float>(i-1)) ) ,
                       Point( bin_w*(i), hist_h - cvRound(hist.at<float>(i)) ),
                       Scalar( 0, 255, 0), 2, 8, 0  );
  }

  namedWindow("Hist image", CV_WINDOW_AUTOSIZE );
  imshow("Hist image", histImage );
  waitKey(0);
}

/**
* This training function outputs histogram models for back- and fore-ground
*/
void train(const std::string& img_path, const std::string& label_path, cv::Mat* back_hist, cv::Mat* fore_hist) {
  cv::Mat img;
  img = cv::imread(img_path, CV_LOAD_IMAGE_GRAYSCALE);

  // Compute the mask
  cv::Mat back_mask(img.rows, img.cols, CV_8UC1, cv::Scalar(0));
  get_mask("background", label_path, &back_mask);

  cv::Mat fore_mask(img.rows, img.cols, CV_8UC1, cv::Scalar(0));
  get_mask("foreground", label_path, &fore_mask);

  // Compute the histogram of training data
  compute_hist(img, back_mask, back_hist);
  compute_hist(img, fore_mask, fore_hist);

  // draw_hist(*fore_hist);
  // draw_hist(*back_hist);
}

/**
* This normalize the hist value into the range of [0,1]
*/
cv::Mat normalize(const cv::Mat& hist) {
  size_t total = 0;
  for (size_t i=0; i<hist.rows; ++i) {
  	float freq = hist.at<float>(i);
  	total += freq;
  }

  cv::Mat n_hist(hist.rows, hist.cols, CV_32FC1, cv::Scalar(0));
  for (size_t i=0; i<n_hist.rows; ++i) {
  	n_hist.at<float>(i) = (float) hist.at<float>(i)/total;
  }

  return n_hist;
}

float boundary_cost(const unsigned char& x1, const unsigned char& x2) {
  float cost = 0.0;

  // // According to [Y. Boykov, 2001]
  // const float sigma = 1000.0;
  // cost = exp(-1.0 * pow((float)x1 - (float)x2, 2.0) / 2*pow(sigma,2.0)) / abs(x1-x2);

  return cost;
}

bool is_valid_neighbor(const cv::Point2i& p, const cv::Mat& img) {
  return (p.x<img.cols) and (p.y<img.rows);
}

class NeighborChecker {
 public:
  NeighborChecker(cv::Mat img) : img_(img) { }
  
  int operator() (const cv::Point2i& p) { 
    return (p.x<img_.cols) and (p.y<img_.rows);
  }

 private:
  cv::Mat img_;
};

std::vector<cv::Point2i> get_neighbors(const cv::Point2i& p, const cv::Mat& img) {
  const size_t max_n_neighbor = 4;

  vector<cv::Point2i> neighbors;
  neighbors.resize(max_n_neighbor);

  cv::Point2i n;

  n = p; n.x++;
  neighbors.at(0) = n;

  n = p; n.x--;
  neighbors.at(1) = n;

  n = p; n.y++;
  neighbors.at(2) = n;

  n = p; n.y--;
  neighbors.at(3) = n;

  NeighborChecker checker(img);
  neighbors.erase( std::remove_if( neighbors.begin(), neighbors.end(), checker), neighbors.end() );

  return neighbors;
} 

LabColorSpace get_labcolor(const cv::Point2i& p, const cv::Mat& img_lab) {
  LabColorSpace c;

  cv::Vec3d pix_bgr = img_lab.ptr<cv::Vec3d>(p.y)[p.x];
  c["L"] = pix_bgr.val[0];
  c["a"] = pix_bgr.val[1];
  c["b"] = pix_bgr.val[2];

  return c;
}

// float l2_norm(const LabColorSpace& p1, const LabColorSpace& p2) {
//   return sqrt( pow(p1["L"]-p2["L"],2) + pow(p1["a"]-p2["a"],2) + pow(p1["b"]-p2["b"],2) );
// }

float l2_norm(LabColorSpace p1, LabColorSpace p2) {
  return sqrt( pow(p1["L"]-p2["L"],2) + pow(p1["a"]-p2["a"],2) + pow(p1["b"]-p2["b"],2) );
}

float l2_norm_avg(const LabColorSpace& p, const std::vector<LabColorSpace>& neighbors) {
  float total = 0.0;

  for (size_t i=0; i<neighbors.size(); ++i) {
    total += l2_norm(p, neighbors.at(i));
  }

  return total/neighbors.size();
}

// According to [B. Packer, 2010]
float boundary_cost(const cv::Point2i& p1, const cv::Point2i& p2, const cv::Mat& img_lab) {
  LabColorSpace p1_lab, p2_lab;
  p1_lab = get_labcolor(p1, img_lab);
  p2_lab = get_labcolor(p2, img_lab);

  std::vector<cv::Point2i> neighbors;
  neighbors = get_neighbors(p1, img_lab);

  std::vector<LabColorSpace> neighbors_lab;
  for (size_t i=0; i<neighbors.size(); ++i)
    neighbors_lab.push_back( get_labcolor(neighbors.at(i), img_lab) );

  float cost = 0.0;
  cost = exp( -1.0 * l2_norm(p1_lab, p2_lab) / (2*l2_norm_avg(p1_lab, neighbors_lab)) );

  return cost;
}

#endif