#ifndef LAB1231_SUN_PRJ_COLOR_MAP_
#define LAB1231_SUN_PRJ_COLOR_MAP_

#include <map>
#include <string>
#include <opencv2/core/core.hpp>
#include <rapidxml-1.13/rapidxml.hpp>
#include <rapidxml-1.13/rapidxml_utils.hpp>
#include <boost/lexical_cast.hpp>

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
 * @brief The coloring scheme follows color map
 */
ClassColorMap voc();

/*!
 * @brief ...
 */
ClassColorMap class_color_map(const std::string dataset);

}// color_map
}// namespace util
}// namespace lab231_sun_prj
#endif