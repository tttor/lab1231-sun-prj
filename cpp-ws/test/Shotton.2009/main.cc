#include <iostream>

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
#include <opengm/opengm.hxx>
#include <opengm/datastructures/marray/marray.hxx>
#include <opengm/graphicalmodel/space/simplediscretespace.hxx>
#include <opengm/graphicalmodel/graphicalmodel.hxx>
#include <opengm/operations/adder.hxx>
#include <opengm/functions/absolute_difference.hxx>

#include <probimage.h>

#include <opengm/operations/minimizer.hxx>
#include <opengm/inference/alphaexpansion.hxx>

inline size_t var_idx(const size_t& x, const size_t& y, const size_t& n_col) {
  return x + n_col * y;
}

int main(int argc, char* argv[]) {
  using namespace cv;
  using namespace std;
  using namespace opengm;

  // Read the (test) image -----------------------------------------------------
  const string image_path = "/home/tor/sun3/dataset/MSRC-21/Msrc/Images/1_17_s.bmp";
  
  cv::Mat image;
  image = cv::imread(image_path, CV_LOAD_IMAGE_COLOR);

  // Construct the GM ----------------------------------------------------------
  cout << "Construct the GM: Begin\n";

  typedef opengm::GraphicalModel<
    float,
    opengm::Adder,
    OPENGM_TYPELIST_1(opengm::ExplicitFunction<float>),
    opengm::SimpleDiscreteSpace<size_t, size_t>
  > Model;
  
  const size_t n_label = 21;
  const size_t n_var = image.rows * image.cols;
  
  Model gm(opengm::SimpleDiscreteSpace<size_t, size_t>(n_var, n_label));
  
  // Add the 1st-order (unary)
  cout << "1st\n";

  ProbImage energy_image;
  energy_image.load( "/home/tor/tmp/result/1_17_s.unary" );
  // energy_image.load( "/home/tor/Downloads/msrc_compressed/1_17_s.c_unary" );
  // energy_image.decompress( "/home/tor/Downloads/msrc_compressed/1_17_s.c_unary" );

  assert(energy_image.width()==image.cols && "err");

  for (size_t x=0; x<image.cols; ++x) {
    for (size_t y=0; y<image.rows; ++y) {
      // add a function
      const size_t shape[] = {n_label};
      opengm::ExplicitFunction<float> unary_energy(shape, shape+1);
      
      for (size_t k=0; k<n_label; ++k) {
        unary_energy(k) = energy_image(x, y, k);
      }

      Model::FunctionIdentifier fid = gm.addFunction(unary_energy);
      
      // add a factor
      size_t var_idxes[] = {var_idx(x, y, image.cols)};
      gm.addFactor(fid, var_idxes, var_idxes+1);
    }
  }

  cout << "Construct the GM: End\n";
  
  // Set up the inference engine -----------------------------------------------
  cout << "Infering: Begin\n";

  // // set up the optimizer (lazy flipper)
  // typedef opengm::LazyFlipper<Model, opengm::Minimizer> LazyFlipperType;
  // LazyFlipperType::VerboseVisitorType visitor;
  // size_t maxSubgraphSize = 5;
  // LazyFlipperType inf_engine(gm, maxSubgraphSize);

  // // set up the optimizer (ICM)
  // typedef opengm::ICM<Model, opengm::Minimizer> IcmType;
  // IcmType::VerboseVisitorType visitor;
  // IcmType inf_engine(gm);

  // //
  // typedef opengm::external::MinSTCutKolmogorov<size_t, float> MinStCutType;
  // typedef opengm::GraphCut<Model, opengm::Minimizer, MinStCutType> GraphCutType;
  // GraphCutType::VerboseVisitorType visitor;
  // GraphCutType inf_engine(gm);

  typedef Model GraphicalModelType;
  typedef opengm::external::MinSTCutKolmogorov<size_t, double> MinStCutType;
  typedef opengm::GraphCut<GraphicalModelType, opengm::Minimizer, MinStCutType> MinGraphCut;
  typedef opengm::AlphaExpansion<GraphicalModelType, MinGraphCut> MinAlphaExpansion;
  MinAlphaExpansion inf_engine(gm);

  // Do infer!
  // inf_engine.infer(visitor);
  inf_engine.infer();
  cout << "Infering: End\n";
  
  // Output --------------------------------------------------------------------
  vector<size_t> labels(n_var);
  inf_engine.arg(labels);

  // size_t idx = 0;
  // for(size_t y = 0; y < image.rows; ++y) {
  //   for(size_t x = 0; x < image.cols; ++x) {
  //      cout << labels[idx] << ' ';
  //      ++idx;
  //   }   
  //   cout << endl;
  // }

  map<size_t, cv::Vec3b> class_color_map;
  
  cv::Vec3b color;
  color[0] = 0; color[1] = 0; color[2] = 0;
  class_color_map[0] = color;
  color[0] = 255; color[1] = 255; color[2] = 255;
  class_color_map[1] = color;
  color[0] = 255; color[1] = 0; color[2] = 0;
  class_color_map[2] = color;
  color[0] = 0; color[1] = 255; color[2] = 0;
  class_color_map[3] = color;
  color[0] = 0; color[1] = 0; color[2] = 255;
  class_color_map[4] = color;
  color[0] = 255; color[1] = 255; color[2] = 0;
  class_color_map[5] = color;
  color[0] = 0; color[1] = 255; color[2] = 255;
  class_color_map[6] = color;
  color[0] = 255; color[1] = 0; color[2] = 255;
  class_color_map[7] = color;
  color[0] = 192; color[1] = 192; color[2] = 192;
  class_color_map[8] = color;
  color[0] = 128; color[1] = 128; color[2] = 128;
  class_color_map[9] = color;
  color[0] = 128; color[1] = 0; color[2] = 0;
  class_color_map[10] = color;
  color[0] = 128; color[1] = 128; color[2] = 0;
  class_color_map[11] = color;
  color[0] = 0; color[1] = 128; color[2] = 0;
  class_color_map[12] = color;
  color[0] = 128; color[1] = 0; color[2] = 128;
  class_color_map[13] = color;
  color[0] = 0; color[1] = 128; color[2] = 128;
  class_color_map[14] = color;
  color[0] = 0; color[1] = 0; color[2] = 128;
  class_color_map[15] = color;
  color[0] = 124; color[1] = 252; color[2] = 0;
  class_color_map[16] = color;
  color[0] = 32; color[1] = 178; color[2] = 170;
  class_color_map[17] = color;
  color[0] = 70; color[1] = 130; color[2] = 180;
  class_color_map[18] = color;
  color[0] = 188; color[1] = 143; color[2] = 143;
  class_color_map[19] = color;
  color[0] = 200; color[1] = 21; color[2] = 133;
  class_color_map[20] = color;

  cv::Mat seg_image(image.rows, image.cols, CV_8UC3, cv::Scalar(0,0,0));
  for (size_t x=0; x<image.cols; ++x) {
    for (size_t y=0; y<image.rows; ++y) {
      size_t label = labels.at(var_idx(x,y,image.cols));

      // //map coloring
      // int colorset[3] = {0,125,255};
      // int countercolor = 0;
      // for(int ii = 0;ii < 3;ii++)
      //   for(int jj = 0; jj<3;jj++)
      //     for (int kk = 0;kk < 3;kk++)
      //     {
      //         if ( labels.at(var_idx(x,y,image.cols))== countercolor )
      //         {
      //           color[0] = colorset[ii]; color[1] = colorset[jj]; color[2] = colorset[kk];
      //         }
      //         countercolor++;
      //     }

      /*if ( labels.at(var_idx(x,y,image.cols))==0 ) {
        color[0] = 0; color[1] = 0; color[2] = 0;
      }
      
      else {
        color[0] = 255; color[1] = 255; color[2] = 255;
      }*/

      seg_image.at<cv::Vec3b>(cv::Point(x,y)) = class_color_map[label];
    }
  }

  const string seg_image_path = "./seg_image_path.jpg";
  cv::imwrite(seg_image_path, seg_image);

  return 0;
}