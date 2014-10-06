#include <opencv2/opencv.hpp>

#include <iostream>
#include <stdio.h>
#include <cmath>
#include <vector>

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

#include <opengm/operations/minimizer.hxx>
#include <opengm/inference/alphaexpansion.hxx>

#include <probimage.h>

#define debug(a,b) cout << a << " " << b << endl 

// The VLFeat header files need to be declared external.
extern "C" {
    #include "vl/generic.h"
    #include "vl/slic.h"
}

using namespace std;
using namespace cv;
using namespace opengm;

//variable SLIC
int region_tb = 100;
int regularization_tb = 1000;//1000;
int minRegion_tb = 40; //10
cv::Mat mat_ori;

//variable for energy unary 
ProbImage energy_image;

//variable untuk openGM dll
size_t n_label = 21; 
size_t n_var;
int n_rows;
int n_cols;
//variable untuk higher order
double theta_h_beta = 12.0;
double theta_alpha = 0.8;
double theta_h_p = 0.2;
double theta_h_v = 0.5;

int getRow(int index){
  return index / n_cols;
}

int getCol(int index){
  return index % n_cols;
}

double unary_feature_label(int row, int col, int k){
  return energy_image(col,row,k);
}

double max_unary_feature(int row, int col){
  double max_val = -1;
  for (int i = 0; i < n_label; i++){
    if ( max_val < unary_feature_label(row,col,i) ){
      max_val = unary_feature_label(row,col,i);
    }
  }
  return max_val;
}

double get_mean(vector<int> superpixel){
  int c = superpixel.size();
  double sum_feature = 0.0;
  
  for(int i = 0; i < c; i++){
    sum_feature += max_unary_feature( getRow(superpixel[i]), getCol(superpixel[i]) );
  }

  return sum_feature / c;
}

double get_mean_label(vector<int> superpixel, int k){
  int c = superpixel.size();
  double sum_feature = 0.0;
  
  for(int i = 0; i < c; i++){
    sum_feature += unary_feature_label( getRow(superpixel[i]), getCol(superpixel[i]), k);
  }

  return sum_feature / c;
}

double func_G(vector<int> superpixel){
  
  int c = superpixel.size();
  double mean_val = get_mean(superpixel);
  // debug("mean val", mean_val);
  double sum_feature = 0.0;
  
  for(int i = 0; i < c; i++){
    sum_feature += pow(max_unary_feature( getRow(superpixel[i]), getCol(superpixel[i]) ) - mean_val, 2);
  }

  // debug("sum_feature",sum_feature);
  double result = -1 * theta_h_beta * ( sum_feature / c );
  
  return exp(result);
}

double func_G_label(vector<int> superpixel, int k){
  
  int c = superpixel.size();
  double mean_val = get_mean_label(superpixel, k);
  double sum_feature = 0.0;
  
  for(int i = 0; i < c; i++){
    sum_feature += pow(unary_feature_label( getRow(superpixel[i]), getCol(superpixel[i]), k) - mean_val, 2);
  }

  double result = -1 * theta_h_beta * ( sum_feature / c  );
  
  return exp(result);
}

double getGamma_Value(int num_pix, double func_g){
  double result = pow(num_pix, theta_alpha) * (theta_h_p + theta_h_v*func_g);
  return result;
}

vector< vector<int> > doSlic(cv::Mat mat){
    
    // Convert image to one-dimensional array.
    float* image = new float[mat.rows*mat.cols*mat.channels()];
    for (int i = 0; i < mat.rows; ++i) {
        for (int j = 0; j < mat.cols; ++j) {
            // Assuming three channels ...
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

    float regularization = 1.0*regularization_tb;
    
    vl_size minRegion = minRegion_tb;

    vl_slic_segment(segmentation, image, width, height, channels, region, regularization, minRegion);

    int size_spixel = 0;
    // segmentation[(mat.cols-1) + (mat.cols*(mat.rows-1))];
    for (int i = 0; i < mat.cols*mat.rows; i++){
      if (size_spixel < segmentation[i]){
        size_spixel=segmentation[i];
      }
    }

    vector< vector<int> >  list_spixel(size_spixel+1);
    
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
    
    cv::imwrite("1_20_s_res.bmp", mat);

    return list_spixel;
}

size_t var_idx(const size_t& x, const size_t& y, const size_t& n_col) {
  return x + n_col * y;
}

float calcNormSquare(cv::Vec3b p1, cv::Vec3b p2){ // return || xi - xj || ^ 2
  float res = 0.0;
  for (int i=0; i < 3; i++){
    res += abs(p1[i] - p2[i]);
  }
  return res*res;
}

cv::Vec2f edge_cost(cv::Point2i p1,cv::Point2i p2, cv::Mat image_rgb, float beta){
  cv::Vec2f result;
  result[0] = 0.0;
  result[1] = 1.0;

  cv::Vec3b xi = image_rgb.at<cv::Vec3b>(p1);
  cv::Vec3b xj = image_rgb.at<cv::Vec3b>(p2); 
  result[0] = exp(-1 * beta * calcNormSquare(xi,xj));
  return result;
}

float getBetaFromImage(cv::Mat image_rgb){
  float total = 0;
  int n_total = 0;
  
  for (size_t x=0; x<image_rgb.cols; ++x) {
    for (size_t y=0; y<image_rgb.rows; ++y) {      
        if (x+1 < image_rgb.cols){
          cv::Vec3b p1 = image_rgb.at<cv::Vec3b>(cv::Point2i(x,y));
          cv::Vec3b p2 = image_rgb.at<cv::Vec3b>(cv::Point2i(x+1,y));       
          total += calcNormSquare(p1,p2);
          ++n_total;
        }
        if (y+1 < image_rgb.rows){
          cv::Vec3b p1 = image_rgb.at<cv::Vec3b>(cv::Point2i(x,y));
          cv::Vec3b p2 = image_rgb.at<cv::Vec3b>(cv::Point2i(x,y+1));       
          total += calcNormSquare(p1,p2);
          ++n_total;
        }      
    }
  }

  float average = total / (float) n_total;
  float result = 1.0 / ( 2.0 * average);
  return result;
}

int main(int argc, char* argv[]) {
  // Read the (test) image -----------------------------------------------------
  const string image_path = "/home/jogie/ws/pairwise_only/build/1_20_s.bmp";
  // energy_image.load("/home/jogie/Downloads/msrc_compressed/1_20_s.c_unary");
  const string target = "/home/jogie/Downloads/msrc_compressed/1_20_s.c_unary";
  energy_image.decompress(target.c_str());

  Mat img_rgb;
  img_rgb = cv::imread(image_path, CV_LOAD_IMAGE_COLOR);

  cv::Mat image;
  image = cv::imread(image_path, CV_LOAD_IMAGE_GRAYSCALE);

  Mat Z = Mat::zeros(image.rows,image.cols, CV_8UC1);

  n_var = image.rows * image.cols;
  
  n_cols = image.cols;
  debug("n cols", n_cols);
  n_rows = image.rows;
  debug("n rows", n_rows);
  
  // Construct the GM ----------------------------------------------------------
  cout << "Construct the GM: Begin\n";

  typedef opengm::GraphicalModel<
    float,
    opengm::Adder,
    // OPENGM_TYPELIST_1(opengm::ExplicitFunction<float>),
    OPENGM_TYPELIST_2(opengm::ExplicitFunction<float>, PottsFunction<float>),
    opengm::SimpleDiscreteSpace<size_t, size_t>
  > Model;    
  
  Model gm(opengm::SimpleDiscreteSpace<size_t, size_t>(n_var, n_label));

  // Add the 1st-order (unary)
  cout << "1st\n";
  for (size_t x=0; x<image.cols; ++x) {
    for (size_t y=0; y<image.rows; ++y) {
      // add a function
      const size_t shape[] = {n_label};
      opengm::ExplicitFunction<float> pen(shape, shape+1);
      
      unsigned char val;
      val = image.at<unsigned char>(y,x);

      // // The function pen(.) penalizes any decision that assigns label 0 for a pixel that should be labelled 1, and the another way around.
      // // Here, it essentially takes the value of probability that the assignment is false.
      // pen(0) = 1.0 - n_back_hist.at<float>(val);
      // pen(1) = 1.0 - n_fore_hist.at<float>(val);
      for(int i = 0; i < n_label; i++){
        pen(i) = -1 * energy_image(x,y,i);
      }

      Model::FunctionIdentifier fid = gm.addFunction(pen);
      
      // add a factor
      size_t var_idxes[] = {var_idx(x, y, image.cols)};
      gm.addFactor(fid, var_idxes, var_idxes+1);
    }
  }

  // Add the 2nd-order (pairwise)
  cout << "2nd\n";  
  
  const float equal_pen = 0.0;
  float unequal_pen = 0.0;

  size_t idx = 0;

  //not yet implement
  float beta = getBetaFromImage(img_rgb);

  for (size_t x=0; x<image.cols; ++x) {
    for (size_t y=0; y<image.rows; ++y) {

      if (x+1 < image.cols) {// (x, y) -- (x + 1, y)
        cv::Point2i p1;   
        p1.x = x; p1.y = y;
        
        cv::Point2i p2;   
        p2.x = x+1; p2.y = y;

        unequal_pen = -1 * ( 45 * (edge_cost(p1, p2, img_rgb, beta)[0]) + 10);

        Z.at<uchar>(y, x) = (uchar)(128 * unequal_pen);
        
        PottsFunction<float> f(n_label, n_label, equal_pen, unequal_pen);
        Model::FunctionIdentifier fid = gm.addFunction(f);

        // add a factor
        size_t var_idxes[] = {var_idx(x,y,image.cols), var_idx(x+1,y,image.cols)};
        sort(var_idxes, var_idxes + 2);
        gm.addFactor(fid, var_idxes, var_idxes + 2);
      }

      if (y+1 < image.rows) {// (x, y) -- (x, y + 1)
        // add a function
        // unequal_pen = boundary_cost(image.at<unsigned char>(y,x),image.at<unsigned char>(y+1,x));
         cv::Point2i p1;   
        p1.x = x; p1.y = y;

        cv::Point2i p2;   
        p2.x = x; p2.y = y+1;
        unequal_pen = -1 * ( 45 * (edge_cost(p1, p2, img_rgb, beta)[0]) + 10);
        
        Z.at<uchar>(y, x) = (uchar)(128 * unequal_pen);

        PottsFunction<float> f(n_label, n_label, equal_pen, unequal_pen);
        Model::FunctionIdentifier fid = gm.addFunction(f);

        // add a factor
        size_t var_idxes[] = {var_idx(x,y,image.cols), var_idx(x,y+1,image.cols)};
        sort(var_idxes, var_idxes + 2);
        gm.addFactor(fid, var_idxes, var_idxes + 2);
      }
    }
  }
  // Add the higher-order (robust P^n model)
  cout << "3rd\n";

  cout << "Construct the GM: End\n";
  cout << "Infering: Begin\n";
  
  // Set up the inference engine -----------------------------------------------

  //set up the optimizer (lazy flipper)
  // typedef opengm::LazyFlipper<Model, opengm::Minimizer> LazyFlipperType;
  // LazyFlipperType::VerboseVisitorType visitor;
  // size_t maxSubgraphSize = 5;
  // LazyFlipperType inf_engine(gm, maxSubgraphSize);

   //set up the optimizer (ICM)
   // typedef opengm::ICM<Model, opengm::Minimizer> IcmType;
   // IcmType::VerboseVisitorType visitor;
   // IcmType inf_engine(gm);

  //
  // typedef opengm::external::MinSTCutKolmogorov<size_t, float> MinStCutType;
  // typedef opengm::GraphCut<Model, opengm::Minimizer, MinStCutType> GraphCutType;
  // GraphCutType::VerboseVisitorType visitor;
  // GraphCutType inf_engine(gm);

  // typedef Model GraphicalModelType;
  // typedef opengm::external::MinSTCutKolmogorov<size_t, double> MinStCutType;
  // typedef opengm::GraphCut<GraphicalModelType, opengm::Minimizer, MinStCutType> MinGraphCut;
  // typedef opengm::AlphaExpansion<GraphicalModelType, MinGraphCut> MinAlphaExpansion;
  // MinAlphaExpansion inf_engine(gm);

  

   // Do infer!
   // inf_engine.infer(visitor);
   // inf_engine.infer();

  // Do infer!
  inf_engine.infer(visitor);
  cout << "Infering: End\n";
  
  // Output --------------------------------------------------------------------
  vector<size_t> labels(n_var);
  inf_engine.arg(labels);

  // size_t 
  // idx = 0;
  // for(size_t y = 0; y < image.rows; ++y) {
  //   for(size_t x = 0; x < image.cols; ++x) {
  //      cout << labels[idx] << ' ';
  //      ++idx;
  //   }   
  //   cout << endl;
  // }


  cv::Mat seg_image(image.rows, image.cols, CV_8UC3, cv::Scalar(0,0,0));
  for (size_t x=0; x<image.cols; ++x) {
    for (size_t y=0; y<image.rows; ++y) {
       // cout << labels.at(var_idx(x,y,image.cols)) << endl;

      cv::Vec3b color;
      if ( labels.at(var_idx(x,y,image.cols))==0 ) {
        color[0] = 0; color[1] = 0; color[2] = 128;
      }
      else 
      if ( labels.at(var_idx(x,y,image.cols))==1 ) {
        color[0] = 0; color[1] = 128; color[2] = 0;
      }
      else 
      if( labels.at(var_idx(x,y,image.cols))== 2 ){
        color[0] = 0; color[1] = 128; color[2] = 128;
      }
      else 
      if( labels.at(var_idx(x,y,image.cols))== 3 ){
        color[0] = 128; color[1] = 0; color[2] = 0;
      }
      else 
      if( labels.at(var_idx(x,y,image.cols))== 4 ){
        color[0] = 128; color[1] = 128; color[2] = 0;
      }
      else 
      if( labels.at(var_idx(x,y,image.cols))== 5 ){
        color[0] = 128; color[1] = 128; color[2] = 128  ;
      }
      else 
      if( labels.at(var_idx(x,y,image.cols))== 6 ){
        color[0] = 0; color[1] = 0; color[2] = 192;
      }
      else 
      if( labels.at(var_idx(x,y,image.cols))== 7 ){
        color[0] = 0; color[1] = 128; color[2] = 64;
      }
      else 
      if( labels.at(var_idx(x,y,image.cols))== 8 ){
        color[0] = 0; color[1] = 128; color[2] = 192;
      }
      else 
      if( labels.at(var_idx(x,y,image.cols))== 9 ){
        color[0] = 128; color[1] = 0; color[2] = 64;
      }
      else 
      if( labels.at(var_idx(x,y,image.cols))== 10 ){
        color[0] = 128; color[1] = 0; color[2] = 192  ;
      }
      else 
      if( labels.at(var_idx(x,y,image.cols))== 11 ){
        color[0] = 128; color[1] = 128; color[2] = 64;
      }
      else 
      if( labels.at(var_idx(x,y,image.cols))== 12 ){
        color[0] = 128; color[1] = 128; color[2] = 192;
      }
      else 
      if( labels.at(var_idx(x,y,image.cols))== 13 ){
        color[0] = 0; color[1] = 64; color[2] = 0;
      }
      else 
      if( labels.at(var_idx(x,y,image.cols))== 14 ){
        color[0] = 0; color[1] = 64; color[2] = 128;
      }
      else 
      if( labels.at(var_idx(x,y,image.cols))== 15 ){
        color[0] = 0; color[1] = 192; color[2] = 0;
      }
      else 
      if( labels.at(var_idx(x,y,image.cols))== 16 ){
        color[0] = 128; color[1] = 64; color[2] = 128;
      }
      else 
      if( labels.at(var_idx(x,y,image.cols))== 17 ){
        color[0] = 128; color[1] = 192; color[2] = 0;
      }
      else 
      if( labels.at(var_idx(x,y,image.cols))== 18 ){
        color[0] = 128; color[1] = 192; color[2] = 128;
      }
      else 
      if( labels.at(var_idx(x,y,image.cols))== 19 ){
        color[0] = 0; color[1] = 64; color[2] = 64;
      }
      else 
      if( labels.at(var_idx(x,y,image.cols))== 20 ){
        color[0] = 0; color[1] = 64; color[2] = 192 ;
      } else {
        color[0] = 0; color[1] = 0; color[2] = 0;
      }     

      seg_image.at<cv::Vec3b>(cv::Point(x,y)) = color;
    }
  }

  const string seg_image_path = "./seg_image_path.jpg";
  cv::imwrite(seg_image_path, seg_image);

  // namedWindow("Display Image", CV_WINDOW_AUTOSIZE );
  // imshow("Display Image", seg_image);

  waitKey(0);

  // mat_ori = cv::imread("/home/jogie/ws/pairwise_only/build/1_20_s.bmp", CV_LOAD_IMAGE_COLOR);
  
  // vector < vector<int> > v = doSlic(mat_ori);
  
  // debug("size list superpixel",v.size());  

  // for (int i = 0; i < v.size(); ++i) {
  //       debug("ukuran spixel",v[i].size());
  //       double fun_g = func_G(v[i]);
  //       cout << "gamma max ke " << i << " : "<< getGamma_Value(v[i].size(), fun_g) << endl;
  //       for (int k = 0; k < n_label; k++){
  //         double fun_g = func_G_label(v[i],k);
  //         cout << "gamma " << k << " " <<  getGamma_Value(v[i].size(), fun_g) << endl;
  //       }
  //       debug("----=-----","");
  // }

  return 0;
}