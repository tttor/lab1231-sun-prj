#ifndef LAB1231_SUN_PRJ_COLOR_MAP_
#define LAB1231_SUN_PRJ_COLOR_MAP_

#include <map>
#include <string>
#include <opencv2/core/core.hpp>

namespace lab1231_sun_prj {
namespace util {
namespace color_map {

typedef
std::map<int, cv::Vec3b>
ClassColorMap;

/*!
 * @brief The coloring scheme follows http://drwn.anu.edu.au/drwnProjMultiSeg.html
 */
ClassColorMap msrc();

/*!
 * @brief ...
 */
ClassColorMap class_color_map(const std::string dataset);

}// color_map
}// namespace util
}// namespace lab231_sun_prj
#endif