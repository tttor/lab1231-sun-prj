#include <iostream>
#include <vector>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <gist/lear_gist_wrapper.h>

int main(int argc, char** argv) {
  using namespace std;
  using namespace lab1231_sun_prj;
  namespace fs = boost::filesystem;
  namespace po = boost::program_options;
  
  string output_dir_path;
  string input_dir_path;
  
  //
  po::options_description desc("Allowed options");
  desc.add_options() 
     ("help", "the help msg")
     ("input_dir", po::value<string>(), "set the input dir")
     ("output_dir", po::value<string>(), "set the out dir")
  ;
  
  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);    
  
  if (vm.count("help")) {
     cout << desc << "\n";
     return 1;
  }
  
  if (vm.count("input_dir")) {
    input_dir_path = vm["input_dir"].as<string>();
    cout << "input_dir was set to " << input_dir_path << "\n";
  } 
  else {
    cout << "input_dir was not set.\n";
    cout << desc << "\n";
    return 1;
  }
  
  if (vm.count("output_dir")) {
    output_dir_path = vm["output_dir"].as<string>();
    if (output_dir_path.substr(output_dir_path.size()-1,1) != "/") {
      output_dir_path += "/";
    }
    cout << "output_dir was set to " << output_dir_path << "\n";
  } 
  else {
    cout << "output_dir was not set.\n";
    cout << desc << "\n";
    return 1;
  }

  //
  fs::create_directories(output_dir_path);
  fs::path img_dir_path(input_dir_path);
  
  for (fs::directory_iterator dir_iter(img_dir_path), end_iter; dir_iter != end_iter; ++dir_iter) {
    LearGistWrapper::extract(dir_iter->path().string(), output_dir_path);  
  }
  
  return 0;
}
