#include "segment_wrapper.h"
#include "pff-segment/pnmfile.h"
#include "pff-segment/segment-image.h"

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
