#include <iostream>
#include <string>
#include <segment/segment_wrapper.h>

int main(int argc, char** argv) {
  using namespace std;
  namespace sun = lab1231_sun_prj;
  
  float sigma = 0.5;
  float k = 500;
  int min_size = 20;
  string img = "/home/tor/robotics/prj/011/ws/seg/pff-seg/img/beach.ppm";
  string out = "/home/tor/robotics/prj/011/ws/seg/pff-seg/img/beach.seg.now.ppm";
  
  sun::pff_segment_wrapper::segment(sigma, k, min_size, img, out);
  
  return 0;
}
