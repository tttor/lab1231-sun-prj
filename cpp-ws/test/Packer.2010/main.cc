#include "util.h"

int main(int argc, char* argv[]) {
  using namespace cv;
  using namespace opengm;

  // Learn from the training data ----------------------------------------------
  // Here we construct histogram models for background and foreground 
  const string tr_image_path = "/home/tor/sun3/dataset/stanford-background-dataset/iccv09Data/favorite/9004294.jpg";
  const string tr_label_path = "/home/tor/sun3/dataset/stanford-background-dataset/iccv09Data/labels/9004294.regions.txt";
  cv::Mat back_hist, fore_hist;
  train(tr_image_path, tr_label_path, &back_hist, &fore_hist);

  cv::Mat n_back_hist, n_fore_hist;
  n_back_hist = normalize(back_hist);
  n_fore_hist = normalize(fore_hist);
  
  // Read the (test) image -----------------------------------------------------
  const string image_path = "/home/tor/sun3/dataset/stanford-background-dataset/iccv09Data/favorite/9004294.jpg";
  
  cv::Mat image;
  image = cv::imread(image_path, CV_LOAD_IMAGE_GRAYSCALE);
  // cout << "image.size()= " << image.size() << endl;
  // cout << "image.channels= " << image.channels() << endl;
  // cv::namedWindow( "Input image", cv::WINDOW_AUTOSIZE );// Create a window for display.
  // cv::imshow( "Input image", image );
  // cv::waitKey(0);// Wait for a keystroke in the window

  cv::Mat img_rgb;
  img_rgb = cv::imread("/home/tor/sun3/dataset/stanford-background-dataset/iccv09Data/favorite/9004294.jpg", CV_LOAD_IMAGE_COLOR);

  cv::Mat img_lab;
  cv::cvtColor(img_rgb, img_lab, CV_BGR2Lab);

  // Construct the GM ----------------------------------------------------------
  cout << "Construct the GM: Begin\n";

  typedef opengm::GraphicalModel<
    float,
    opengm::Adder,
    OPENGM_TYPELIST_2(opengm::ExplicitFunction<float>, PottsFunction<float>),
    opengm::SimpleDiscreteSpace<size_t, size_t>
  > Model;
  
  const size_t n_label = 2; // {0=background, 1=foreground}
  const size_t n_var = image.rows * image.cols;
  
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

      // The function pen(.) penalizes any decision that assigns label 0 for a pixel that should be labelled 1, and the another way around.
      // Here, it essentially takes the value of probability that the assignment is false.
      pen(0) = 1.0 - n_back_hist.at<float>(val);
      pen(1) = 1.0 - n_fore_hist.at<float>(val);
      
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
  for (size_t x=0; x<image.cols; ++x) {
    for (size_t y=0; y<image.rows; ++y) {
      cout << "idx= " << idx +1 << " of " << image.cols*image.rows << endl;
      ++idx;

      cv::Point2i p1;
      p1.x = x; p1.y = y;

      if (x+1 < image.cols) {// (x, y) -- (x + 1, y)
        // add a function
        // unequal_pen = boundary_cost(image.at<unsigned char>(y,x),image.at<unsigned char>(y,x+1));

        cv::Point2i p2;
        p2.x = x+1; p2.y = y;
        unequal_pen = boundary_cost(p1, p2, img_lab);
        
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

        cv::Point2i p2;   
        p2.x = x; p2.y = y+1;
        unequal_pen = boundary_cost(p1, p2, img_lab);
        
        PottsFunction<float> f(n_label, n_label, equal_pen, unequal_pen);
        Model::FunctionIdentifier fid = gm.addFunction(f);

        // add a factor
        size_t var_idxes[] = {var_idx(x,y,image.cols), var_idx(x,y+1,image.cols)};
        sort(var_idxes, var_idxes + 2);
        gm.addFactor(fid, var_idxes, var_idxes + 2);
      }
    }
  }
  
  cout << "Construct the GM: End\n";
  cout << "Infering: Begin\n";
  
  // Set up the inference engine -----------------------------------------------

  // // set up the optimizer (lazy flipper)
  // typedef opengm::LazyFlipper<Model, opengm::Minimizer> LazyFlipperType;
  // LazyFlipperType::VerboseVisitorType visitor;
  // size_t maxSubgraphSize = 5;
  // LazyFlipperType inf_engine(gm, maxSubgraphSize);

  // set up the optimizer (ICM)
  typedef opengm::ICM<Model, opengm::Minimizer> IcmType;
  IcmType::VerboseVisitorType visitor;
  IcmType inf_engine(gm);

  // //
  // typedef opengm::external::MinSTCutKolmogorov<size_t, float> MinStCutType;
  // typedef opengm::GraphCut<Model, opengm::Minimizer, MinStCutType> GraphCutType;
  // GraphCutType::VerboseVisitorType visitor;
  // GraphCutType inf_engine(gm);

  // Do infer!
  inf_engine.infer(visitor);
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

  cv::Mat seg_image(image.rows, image.cols, CV_8UC3, cv::Scalar(0,0,0));
  for (size_t x=0; x<image.cols; ++x) {
    for (size_t y=0; y<image.rows; ++y) {
      cv::Vec3b color;
      if ( labels.at(var_idx(x,y,image.cols))==0 ) {
        color[0] = 0; color[1] = 0; color[2] = 0;
      }
      else {
        color[0] = 255; color[1] = 255; color[2] = 255;
      }

      seg_image.at<cv::Vec3b>(cv::Point(x,y)) = color;
    }
  }

  const string seg_image_path = "./seg_image_path.jpg";
  cv::imwrite(seg_image_path, seg_image);

  return 0;
}