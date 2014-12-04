#include <util/color_map.h>

namespace sun = lab1231_sun_prj;

sun::util::color_map::ClassColorMap sun::util::color_map::class_color_map(const std::string& dataset) {
  if (dataset=="msrc")
    return sun::util::color_map::msrc();
  if (dataset=="voc2010" or dataset=="voc2012")
    return sun::util::color_map::voc();
  else
    assert(false && "color_map(): UNKNOWN DATASET!");
}

sun::util::color_map::ClassColorMap sun::util::color_map::voc() {
  using namespace std;
  using namespace rapidxml;

  sun::util::color_map::ClassColorMap map;
  cv::Vec3b color;

  rapidxml::file<> xmlFile("/home/rizkivmaster/ws/lab1231-sun-prj/util/config/class_color_map.xml");
  rapidxml::xml_document<> doc;
  doc.parse<0>(xmlFile.data());

  xml_node<>* root = doc.first_node();
  for(xml_node<>* sub=root->first_node("class"); sub; sub=sub->next_sibling()) {
    xml_attribute<>* sub_att_1 = sub->first_attribute("num");
    size_t num = boost::lexical_cast<size_t>(sub_att_1->value());

    for(xml_node<>* subsub=sub->first_node("color"); subsub; subsub=subsub->next_sibling()) {
      xml_attribute<>* subsub_att_r = subsub->first_attribute("r");
      size_t r = boost::lexical_cast<size_t>(subsub_att_r->value());

      xml_attribute<>* subsub_att_g = subsub->first_attribute("g");
      size_t g = boost::lexical_cast<size_t>(subsub_att_g->value());

      xml_attribute<>* subsub_att_b = subsub->first_attribute("b");
      size_t b = boost::lexical_cast<size_t>(subsub_att_b->value());

      cv::Vec3b color;
      color[0] = b; color[1] = g; color[2] = r;

      map[num] = color;
    }
  }

  return map;
}

sun::util::color_map::ClassColorMap sun::util::color_map::msrc() {
  sun::util::color_map::ClassColorMap map;
  cv::Vec3b color;
  
  // -1 -> void
  color[0] = 0; color[1] = 0; color[2] = 0;
  map[-1] = color;

  // 0 -> building
  color[0] = 0; color[1] = 0; color[2] = 128;
  map[0] = color;

  // 1 -> grass
  color[0] = 0; color[1] = 128; color[2] = 0;
  map[1] = color;

  // 2 -> tree
  color[0] = 0; color[1] = 128; color[2] = 128;
  map[2] = color;

  // 3 -> cow
  color[0] = 128; color[1] = 0; color[2] = 0;
  map[3] = color;

  // 4-> sheep
  color[0] = 128; color[1] = 128; color[2] = 0;
  map[4] = color;

  // 5-> sky
  color[0] = 128; color[1] = 128; color[2] = 128;
  map[5] = color;

  // 6 -> airplane
  color[0] = 0; color[1] = 0; color[2] = 192;
  map[6] = color;

  // 7 -> water
  color[0] = 0; color[1] = 128; color[2] = 64;
  map[7] = color;

  // 8 -> face
  color[0] = 0; color[1] = 128; color[2] = 192;
  map[8] = color;

  // 9 -> car
  color[0] = 128; color[1] = 0; color[2] = 64;
  map[9] = color;

  // 10 -> bicycle
  color[0] = 128; color[1] = 0; color[2] = 192;
  map[10] = color;

  // 11 -> flower
  color[0] = 128; color[1] = 128; color[2] = 64;
  map[11] = color;

  // 12 ->
  color[0] = 128; color[1] = 128; color[2] = 192;
  map[12] = color;

  // 13->
  color[0] = 0; color[1] = 64; color[2] = 0;
  map[13] = color;

  // 14 ->
  color[0] = 0; color[1] = 64; color[2] = 128;
  map[14] = color;

  color[0] = 0; color[1] = 192; color[2] = 0;
  map[15] = color;

  color[0] = 128; color[1] = 64; color[2] = 128;
  map[16] = color;

  color[0] = 128; color[1] = 192; color[2] = 0;
  map[17] = color;

  color[0] = 128; color[1] = 192; color[2] = 128;
  map[18] = color;

  color[0] = 0; color[1] = 64; color[2] = 64;
  map[19] = color;

  color[0] = 0; color[1] = 64; color[2] = 192 ;
  map[20] = color;

  return map;
}