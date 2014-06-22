#include <iostream>
#include <vector>
#include <boost/filesystem.hpp>
#include <gist/lear_gist_wrapper.h>

int main(int argc, char** argv) {
  using namespace std;
  using namespace lab1231_sun_prj;
  namespace fs = boost::filesystem;
  
  cout << "Hallo :)\n";
  
  string out_dir_path = "../out/gist/";
  fs::create_directories(out_dir_path);
  
  fs::path img_dir_path(string("/home/tor/robotics/prj/011/ws/oliv-sun/spatial_envelope_256x256_static_8outdoorcategories"));
  
  for (fs::directory_iterator dir_iter(img_dir_path), end_iter; dir_iter != end_iter; ++dir_iter) {
    LearGistWrapper::extract(dir_iter->path().string(), out_dir_path);  
  }
  
  return 0;
}
