#ifndef SVM_STRUCT_SS_IO_H
#define SVM_STRUCT_SS_IO_H

#include <vector>
#include <fstream>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/tokenizer.hpp>
#include <Eigen/Dense>

// #define DEBUG_ON

#ifdef DEBUG_ON
#define debug_var(name,value) std::cout<<name<<"= "<<value<<std::endl
#define debug_msg(msg) std::cout<<msg<<std::endl
#define debug_in_msg(funct) std::cout<<funct<<"(): BEGIN"<<std::endl
#define debug_out_msg(funct) std::cout<<funct<<"(): END"<<std::endl
#else
#define debug_var(name,value) {}
#define debug_msg(msg) {}
#define debug_in_msg(funct) {}
#define debug_out_msg(funct) {}
#endif

#define fatal_print(name,value) std::cout<<name<<value<<std::endl

namespace svm_struct_ss {
namespace io {

std::vector<std::string> read_list(const std::string& list_filepath) {
  using namespace std;
  
  vector<string> list;

  ifstream list_file(list_filepath.c_str());
  if (list_file.is_open()) {
    string line;
    while ( getline(list_file,line) ) {
      const string last = line.substr(line.size()-1);
      if (last=="\r" or last=="\n") line = line.substr(0,line.size()-1);
      list.push_back(line);
    }
    list_file.close();
  }
  else {
    fatal_print("list_filepath= ",list_filepath);
    assert(false && "cannot open list_file");
  }

  return list;
}

template<typename MatType>
MatType read_csv(const std::string& csv_path) {
  // Adopted from: https://gist.github.com/stephenjbarr/2266900
  using namespace std;
  using namespace boost;
  
  ifstream csv(csv_path.c_str());
  BOOST_ASSERT_MSG(csv.is_open(), string("ERROR: Cannot find: " + csv_path).c_str());
  
  typedef tokenizer< escaped_list_separator<char> > Tokenizer;
  
  vector< string > vec;
  string line;
  vector< vector< string > > matrows;
  
  while (getline(csv,line)) {
    Tokenizer tok(line);
    vec.assign(tok.begin(),tok.end());
    matrows.push_back(vec);
  }
  csv.close();
  
  // FIGURE OUT HOW MANY OF THE ROWS HAVE THE RIGHT NUMBER
  // OF COLUMNS
  int Nrows = matrows.size();
  int Ncols = matrows[0].size();
  int Ngoodrows = 0;
  for(int i = 0; i < Nrows; i++) {
    if(matrows[i].size() == Ncols) {
      Ngoodrows++;
    }
  }
  
  // TRANSFORM THE VECTOR OF ROWS INTO AN EIGEN DOUBLE MATRIX
  MatType xmat = MatType(Ngoodrows, Ncols);
  //cout << "INPUT MATRIX: " << Nrows << "x" << Ncols << endl;
  int rc = 0;
  for(int i = 0; i < Nrows; i++) {
    int rowsize = matrows[i].size();
    if(rowsize != Ncols) {
      cout << "WARNING: Row " << i << " has bad column count" << endl;
      continue;
    } 
    
    for(int j = 0; j < Ncols; j++) {
      xmat(rc,j) = int( round(strtod(matrows[i][j].c_str(), NULL)) );
      xmat(rc,j) = lexical_cast<double>(matrows[i][j]);
    }
    rc++;
  }
  
  return xmat;
}

template<typename MatType>
void write_csv(const MatType m, const std::string csv_path) {
  using namespace std;
  using namespace boost;

  ofstream csv;
  csv.open(csv_path.c_str());
  BOOST_ASSERT_MSG(csv.is_open(), string("ERROR: Cannot open: " + csv_path).c_str());

  for(uint64_t  i=0; i<m.rows(); i++){
    for(uint64_t j=0; j<m.cols(); j++){
      string str = lexical_cast<std::string>(m(i,j));
      if(j+1 == m.cols()) csv << str;
      else csv << str << ",";
    }
    csv << "\n";
  }

  csv.close();  
}

}  
}

#endif
