#include <iostream>
#include <stdio.h>
#include <cmath>
#include <vector>

#include <opencv2/opencv.hpp>

#include <segmenter/segment_wrapper.h>
#include <pff-segment/pnmfile.h>
#include <pff-segment/segment-image.h>
// The VLFeat header files need to be declared external.
extern "C" {
    #include <vlfeat-0.9.19/vl/generic.h>
    #include <vlfeat-0.9.19/vl/slic.h>
}

#include <util/util.h>

namespace sun = lab1231_sun_prj;

void sun::pff_segment_wrapper::segment(const float& sigma, const float& k, const int& min_size, const std::string& in_path, const std::string& out_path) {
  // loading input image
  image<rgb> *input = loadPPM(in_path.c_str());

  // processing
  int num_ccs; 
  image<rgb> *seg = segment_image(input, sigma, k, min_size, &num_ccs); 
  
  //
  savePPM(seg, out_path.c_str());
}

std::vector<sun::util::Superpixel> sun::slic_segment_wrapper::segment(cv::Mat mat, int region_tb, double regularization_tb, int minRegion_tb) {
  using namespace std;

  float* image = new float[mat.rows*mat.cols*mat.channels()];
  for (int i = 0; i < mat.rows; ++i) {
      for (int j = 0; j < mat.cols; ++j) {
          image[j + mat.cols*i + mat.cols*mat.rows*0] = mat.at<cv::Vec3b>(i, j)[0];
          image[j + mat.cols*i + mat.cols*mat.rows*1] = mat.at<cv::Vec3b>(i, j)[1];
          image[j + mat.cols*i + mat.cols*mat.rows*2] = mat.at<cv::Vec3b>(i, j)[2];
      }
  }

  // The algorithm will store the final segmentation in a one-dimensional array.
  vl_uint32* segmentation = new vl_uint32[mat.rows*mat.cols];
  vl_size height = mat.rows;
  vl_size width = mat.cols;
  vl_size channels = mat.channels();

  // The region size defines the number of superpixels obtained.
  // Regularization describes a trade-off between the color term and the
  // spatial term.
  vl_size region = region_tb;
  double regularization = regularization_tb;    
  vl_size minRegion = minRegion_tb;

  vl_slic_segment(segmentation, image, width, height, channels, region, regularization, minRegion);

  int size_spixel = 0; //get max element
  for (int i = 0; i < mat.cols*mat.rows; i++){      
    if (size_spixel < segmentation[i]){
      size_spixel=segmentation[i];
    }
  }

  vector<sun::util::Superpixel> list_spixel(size_spixel+1);
  for (int i = 0; i < mat.rows; ++i) {
      //labels[i] = new int[mat.cols];
      for (int j = 0; j < mat.cols; ++j) {            
          int spixel_index = (int) segmentation[j + mat.cols*i];
          list_spixel[spixel_index].push_back(j + mat.cols*i);
      }
  }

  // Convert segmentation.
  int** labels = new int*[mat.rows];
  for (int i = 0; i < mat.rows; ++i) {
      labels[i] = new int[mat.cols];
              
      for (int j = 0; j < mat.cols; ++j) {
          labels[i][j] = (int) segmentation[j + mat.cols*i];
      }
  }

  int label = 0;
  int labelTop = -1;
  int labelBottom = -1;
  int labelLeft = -1;
  int labelRight = -1;

  for (int i = 0; i < mat.rows; i++) {
      for (int j = 0; j < mat.cols; j++) {
          
          label = labels[i][j];

          labelTop = label;
          if (i > 0) {
              labelTop = labels[i - 1][j];
          }
          
          labelBottom = label;
          if (i < mat.rows - 1) {
              labelBottom = labels[i + 1][j];
          }
          
          labelLeft = label;
          if (j > 0) {
              labelLeft = labels[i][j - 1];
          }
          
          labelRight = label;
          if (j < mat.cols - 1) {
              labelRight = labels[i][j + 1];
          }
          
          if (label != labelTop || label != labelBottom || label!= labelLeft || label != labelRight) {
              mat.at<cv::Vec3b>(i, j)[0] = 0;
              mat.at<cv::Vec3b>(i, j)[1] = 0;
              mat.at<cv::Vec3b>(i, j)[2] = 255;
          }
      }
  }

  #ifdef DEBUG_LEVEL_2
  cv::imwrite("slic.bmp", mat);
  #endif

  return list_spixel;  
}