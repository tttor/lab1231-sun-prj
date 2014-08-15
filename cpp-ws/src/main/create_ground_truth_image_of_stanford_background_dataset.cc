/**
* To create ground truth segmented images from Stanford Background Dataset
*/
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <boost/algorithm/string.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include <map>

int main(int argc, char* argv[])
{
  using namespace std;
  using namespace cv;
  
  assert(argc == 4 && "argc != 4");
  
  const string image_path = argv[1];
  const string label_dir = argv[2];
  const string out_dir = argv[3];
  
  const size_t image_name_len = 7;// Assume this
  const size_t extension_len = 4;// .jpg
  assert(image_path.size() > image_name_len+extension_len);
  const string image_name = image_path.substr(image_path.size()-image_name_len-extension_len, image_name_len);
  const string label_path = string(label_dir + "/" + image_name + ".regions.txt");
  const string seg_image_path = string(out_dir + "/" + image_name + ".groundtruthseg.jpg");
  
  // Read the image and label
  cv::Mat image;
  image = cv::imread(image_path,0);
  
  
  ifstream label_file(label_path.c_str());
  vector< vector<string> > label_str;
  
  if (label_file.is_open()) {
    string line;
    while ( getline(label_file,line) )
    {
      //cout << line << '\n';
      
      vector<string> label_chars;
      boost::split( label_chars, line, boost::algorithm::is_any_of(" "), boost::token_compress_on );
      
      label_str.push_back(label_chars);
    }
    label_file.close();
  }
  else {
    cout << "Unable to open file";
  } 
  
  assert(label_str.size() == image.rows && "lines.size() != image.rows");
  
  // Set
  map<string, string> class_name;
  class_name["0"] = "sky";
  class_name["1"] = "tree";
  class_name["2"] = "road";
  class_name["3"] = "grass";
  class_name["4"] = "water";
  class_name["5"] = "bldg";
  class_name["6"] = "mntn";
  class_name["7"] = "fg obj.";
  
  map<string, cv::Vec3b> class_color;
  Vec3b color;
  
  color[0] = 117; color[1] = 117; color[2] = 117;
  class_color["sky"] = color;
  color[0] = 39; color[1] = 103; color[2] = 90;
  class_color["tree"] = color;
  color[0] = 68; color[1] = 0; color[2] = 68;
  class_color["road"] = color;
  color[0] = 38; color[1] = 173; color[2] = 64;
  class_color["grass"] = color;
  color[0] = 142; color[1] = 0; color[2] = 0;
  class_color["water"] = color;
  color[0] = 13; color[1] = 28; color[2] = 94;
  class_color["bldg"] = color;
  color[0] = 0; color[1] = 0; color[2] = 0;
  class_color["mntn"] = color;
  color[0] = 0; color[1] = 142; color[2] = 255;
  class_color["fg obj."] = color;
  
  //
  cv::Mat seg_image(image.rows, image.cols, CV_8UC3, cv::Scalar(0,0,0));
  
  for (size_t i=0; i<seg_image.rows; ++i) {
    assert(label_str.at(i).size() == seg_image.cols && "label_str.at(i).size()!= seg_image.cols");

    for (size_t j=0; j<seg_image.cols; ++j) {
      seg_image.at<Vec3b>(Point(j,i)) = class_color[class_name[label_str.at(i).at(j)]];
    }
  }
  
  cv::imwrite(seg_image_path, seg_image);
    
  return 0;
}
