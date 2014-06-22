#include "lear_gist_wrapper.h"

using namespace lab1231_sun_prj;

void LearGistWrapper::extract(const std::string& img_path, const std::string& out_dir_path) {
  using namespace std;
  using namespace Eigen;
  using namespace boost;
  cout << "Extracting GIST of " << img_path<< endl;
  
  //
  vector<string> img_path_parts;
  split(img_path_parts, img_path, boost::algorithm::is_any_of("/"), token_compress_on);
  
  const size_t dot_pos = img_path_parts.back().find('.');
  const string ext = img_path_parts.back().substr(dot_pos+1, img_path_parts.back().size());
  
  const string img_filename = img_path_parts.back().substr(0, img_path_parts.back().size()-dot_pos+1);
  
  string ppm_img_path;
  if (ext != "ppm") {
    cv::Mat not_ppm_img= cv::imread(img_path);
    
    const string ppm_img_out_dir = img_path.substr(0, img_path.size()-img_path_parts.back().size());
    const string c_ppm_img_path = ppm_img_out_dir + img_filename + ".ppm";
    cv::imwrite(c_ppm_img_path, not_ppm_img);
    
    ppm_img_path = c_ppm_img_path;
  }
  else {
    ppm_img_path = img_path;
  }
  
  //
  int nblocks=4;
  int n_scale=3;
  int orientations_per_scale[50]={8,8,4};
  
  color_image_t* img;
  img = load_ppm(ppm_img_path.c_str());

  float* gist;
  gist = color_gist_scaletab(img,nblocks,n_scale,orientations_per_scale);
  
  /* compute descriptor size */
  int gist_size=0;
  for(uint64_t j=0; j<n_scale; j++) {
    gist_size += nblocks*nblocks*orientations_per_scale[j];
  }
  gist_size *= 3; /* color */
  
  // Convert the desc to Eigen::Vector
  VectorXd gist_vec(gist_size);
  for (uint64_t j=0; j<gist_size; j++) {
    gist_vec(j) = gist[j];
  } 
  
  //
  
  EigenlibSupport::write(gist_vec, string(out_dir_path+img_filename+".gist"));
  
  //
  free(gist);
  color_image_delete(img);
}

color_image_t* LearGistWrapper::load_ppm(const char *fname) {
  FILE *f=fopen(fname,"r");
  if(!f) {
    perror("could not open infile");
    exit(1);
  }
  int px,width,height,maxval;
  if(fscanf(f,"P%d %d %d %d",&px,&width,&height,&maxval)!=4 || 
     maxval!=255 || (px!=6 && px!=5)) {
    fprintf(stderr,"Error: input not a raw PGM/PPM with maxval 255\n");
    exit(1);    
  }
  fgetc(f); /* eat the newline */
  color_image_t *im=color_image_new(width,height);

  int i;
  for(i=0;i<width*height;i++) {
    im->c1[i]=fgetc(f);
    if(px==6) {
      im->c2[i]=fgetc(f);
      im->c3[i]=fgetc(f);    
    } else {
      im->c2[i]=im->c1[i];
      im->c3[i]=im->c1[i];   
    }
  }
  
  fclose(f);
  return im;
}
