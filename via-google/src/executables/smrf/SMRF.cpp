#include <iostream>
#include <string.h>

#include "saliency/saliency.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <opengm/graphicalmodel/graphicalmodel.hxx>
#include <opengm/graphicalmodel/space/simplediscretespace.hxx>
#include <opengm/functions/potts.hxx>
#include <opengm/operations/adder.hxx>
#include <opengm/inference/graphcut.hxx>
#include <opengm/inference/auxiliary/minstcutkolmogorov.hxx>
#include <opengm/opengm.hxx>
#include <opengm/datastructures/marray/marray.hxx>
#include <opengm/graphicalmodel/space/simplediscretespace.hxx>
#include <opengm/graphicalmodel/graphicalmodel.hxx>
#include <opengm/operations/adder.hxx>
#include <opengm/functions/absolute_difference.hxx>

#include <opengm/operations/minimizer.hxx>
#include <opengm/inference/alphaexpansion.hxx>

//COLOR_NORMALIZER = sqrt(3*(255**2)
const double COLOR_NORMALIZER = 441.67295593006367;

size_t var_idx(const size_t& x, const size_t& y, const size_t& n_col) {
	return x + n_col * y;
}

double colorDistance(Vec3b c1,Vec3b c2)
{
	return norm(c1-c2,NORM_L2)/COLOR_NORMALIZER;
	//return norm(c1-c2,NORM_L2);
}

using namespace cv;
using namespace std;
using namespace opengm;

//arguments structures: SMRF nameinput nameoutput
int main(int argc, char* argv[]) {


  // Read the (test) image -----------------------------------------------------
	if(argc<3)
	{
		printf("Please add one filename as input and one filename as output\n");
		return 0;
	}

	std::string srcdir = argv[1];
	std::string tgtdir = argv[2];
	float alpha = atof(argv[3]);
	float beta = atof(argv[4]);
	int classlabel = atoi(argv[5]);

	Mat src = imread(srcdir);
	if(src.cols>1024 || src.rows>1024)
	{
		float scale = 1.0;
		if(src.cols>src.rows)
		{
			scale = 1024.0/src.cols;
		}
		else
		{
			scale = 1024.0/src.rows;
		}
		int tgtwidth = (int)(scale*src.cols);
		int tgtheight = (int)(scale*src.rows);
		Mat des_img;
    cv::resize(src,des_img,cv::Size2i(tgtwidth, tgtheight));
    src = des_img;
	}

	Saliency saliency;
	Mat_<double> saliencyMap = saliency.saliency(src);
	imwrite("debug_sal.png",saliencyMap*255);

  	// Construct the GM ----------------------------------------------------------
	cout << "Construct the GM: Begin\n";

	//DEFINE THE NEIGHBORHOOD SYSTEM
	//define the nodes
	const int numberOfVariables = src.cols*src.rows;
	const int numberOfLabels = 2;

	assert(src.cols*src.rows > 0);
	assert(src.cols == saliencyMap.cols && src.rows == saliencyMap.rows);

	typedef SimpleDiscreteSpace<size_t, size_t> Space;
	Space space(numberOfVariables, numberOfLabels);

	//define the connectivity through energy function
	typedef OPENGM_TYPELIST_2(ExplicitFunction<double>, PottsFunction<double>) FunctionTypelist;

	//construct the model
	typedef GraphicalModel<double, Adder, FunctionTypelist, Space> Model;

	//model construction
	Model gm(space);

	cout << "Alright\n";
	
	//UNARY time!!!	
	for (size_t x=0; x<(size_t)saliencyMap.cols; ++x) {
		for (size_t y=0; y<(size_t)saliencyMap.rows; ++y) {
			const size_t shape[] = {numberOfLabels};
			//UNARY VARIABLE
      		// add a function for unary operator
			ExplicitFunction<double> unary_energy(shape, shape+1);
			//define the domain and co-domain
			assert(saliencyMap.at<double>(y,x)>=0 &&saliencyMap.at<double>(y,x)<=1);
			unary_energy(0) = alpha*saliencyMap.at<double>(y,x);
			unary_energy(1) = (1-saliencyMap.at<double>(y,x));
			//set id of function
			Model::FunctionIdentifier fid = gm.addFunction(unary_energy);
      		// add a factor
			size_t var_ids[] = {var_idx(x, y, saliencyMap.cols)};
			// printf("d: %d \n",var_idx(x, y, saliencyMap.cols));
			gm.addFactor(fid, var_ids, var_ids+1);

		}
	}
	cout << "Alright\n";
	
	//PAIR-WISE
	for (size_t x = 0; x<(size_t)src.cols-1;++x){
		for(size_t y = 0;y<(size_t)src.rows-1;++y){
			//PAIR-WISE
			//get color distance
			Vec3b cc = src.at<Vec3b>(y,x);
			Vec3b cy = src.at<Vec3b>(y+1,x);
			Vec3b cx = src.at<Vec3b>(y,x+1);
			double dy = colorDistance(cc,cy);
			double dx = colorDistance(cc,cx);
			//allocate color distance
			PottsFunction<double> pairX_energy(numberOfLabels,numberOfLabels,0,beta*dx);
			PottsFunction<double> pairY_energy(numberOfLabels,numberOfLabels,0,beta*dy);
			Model::FunctionIdentifier fxid = gm.addFunction(pairX_energy);
			Model::FunctionIdentifier fyid = gm.addFunction(pairY_energy);
			size_t var_idxes[] = {var_idx(x,y,src.cols),var_idx(x+1,y,src.cols)};
			size_t var_idyes[] = {var_idx(x,y,src.cols),var_idx(x,y+1,src.cols)};
			sort(var_idxes,var_idxes+2);
			sort(var_idyes,var_idyes+2);
			gm.addFactor(fxid,var_idxes,var_idxes+2);
			gm.addFactor(fyid,var_idyes,var_idyes+2);
		}
	}
	cout << "Construct the GM: End\n";

	// INFERENCE
	cout << "Infering: Begin\n";

	//set problem type
	typedef external::MinSTCutKolmogorov<size_t,double> MinStCutType;
	//set solution type
	typedef GraphCut<Model,opengm::Minimizer,MinStCutType> MinCut;
	//set verbosal computation
	MinCut::VerboseVisitorType visitor;
	//instantiate inference enginer
	MinCut mincut(gm);
	//start to infer
	mincut.infer(visitor);
	//show value
	cout << "value: " <<mincut.value()  << endl;
	cout << "Infering: End\n";

	//SEGMENTATION
	vector<size_t> labels(numberOfVariables);
	//inquiry segmentation
	mincut.arg(labels);
	//mapping output
	Mat_<int> output(src.rows,src.cols);
	Mat_<int> debug(src.rows,src.cols);

	for (size_t x=0; x<(size_t)src.cols; ++x) {
		for (size_t y=0; y<(size_t)src.rows; ++y) {
			if(labels.at(var_idx(x,y,src.cols))==1)
			{
				output.at<int>(y,x) = classlabel;
			}
			else
			{
				output.at<int>(y,x) = 255;	
			}
			debug.at<int>(y,x) = labels.at(var_idx(x,y,src.cols))*255;
		}
	}
	//write to disk
	imwrite(tgtdir,output);
	imwrite(srcdir,src);
	imwrite(tgtdir+"sal.jpg",saliencyMap*255);	
	//imwrite(tgtdir+"debug.jpg",debug);	
	return 0;
}