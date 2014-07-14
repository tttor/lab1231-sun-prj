#ifndef LAB1231_SUN_PRJ_SEGMENT_WRAPPER_H_
#define LAB1231_SUN_PRJ_SEGMENT_WRAPPER_H_

#include <string>
#include <cstdio>
#include <cstdlib>

namespace lab1231_sun_prj {

namespace pff_segment_wrapper {

//! The core process is adapted from ./pff-segment/segment.cpp
void segment(const float& sigma, const float& k, const int& min_size, const std::string& in_path, const std::string& out_path);

}// namespace pff_segment

}// namespace lab1231_sun_prj

#endif
