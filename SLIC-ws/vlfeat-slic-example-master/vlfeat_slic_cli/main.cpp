// OpenCV can be used to read images.
#include <opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>

#define debug(a) cout << "test " << a << endl 

// The VLFeat header files need to be declared external.
extern "C" {
    #include "vl/generic.h"
    #include "vl/slic.h"
}
using namespace std;
using namespace cv;

int region_tb = 50;
int regularization_tb = 1000;//1000;
int minRegion_tb = 10; //10
cv::Mat mat_ori;

cv::Mat doSlic(cv::Mat mat){
    debug("init");
    cout << "MyCallbackForContrast : Region = "<< region_tb << " " << 1*regularization_tb << " " << minRegion_tb<<endl;
    // Convert image to one-dimensional array.
    float* image = new float[mat.rows*mat.cols*mat.channels()];
    for (int i = 0; i < mat.rows; ++i) {
        for (int j = 0; j < mat.cols; ++j) {
            // Assuming three channels ...
            image[j + mat.cols*i + mat.cols*mat.rows*0] = mat.at<cv::Vec3b>(i, j)[0];
            image[j + mat.cols*i + mat.cols*mat.rows*1] = mat.at<cv::Vec3b>(i, j)[1];
            image[j + mat.cols*i + mat.cols*mat.rows*2] = mat.at<cv::Vec3b>(i, j)[2];
        }
    }

    debug("2");
    // The algorithm will store the final segmentation in a one-dimensional array.
    vl_uint32* segmentation = new vl_uint32[mat.rows*mat.cols];
    vl_size height = mat.rows;
    vl_size width = mat.cols;
    vl_size channels = mat.channels();
            
    // The region size defines the number of superpixels obtained.
    // Regularization describes a trade-off between the color term and the
    // spatial term.
    vl_size region = region_tb;

    float regularization = 1.0*regularization_tb;
    
    vl_size minRegion = minRegion_tb;
    
    debug("start segment");

    vl_slic_segment(segmentation, image, width, height, channels, region, regularization, minRegion);
            
    // Convert segmentation.
    int** labels = new int*[mat.rows];
    for (int i = 0; i < mat.rows; ++i) {
        labels[i] = new int[mat.cols];
                
        for (int j = 0; j < mat.cols; ++j) {
            labels[i][j] = (int) segmentation[j + mat.cols*i];
        }
    }
    
    debug("end segment");

    int label = 0;
    int labelTop = -1;
    int labelBottom = -1;
    int labelLeft = -1;
    int labelRight = -1;
    
    for (int i = 0; i < mat.rows; i++) {
        for (int j = 0; j < mat.cols; j++) {
            
            label = labels[i][j];
            
            labelTop = label;
            if (i > 0) {
                labelTop = labels[i - 1][j];
            }
            
            labelBottom = label;
            if (i < mat.rows - 1) {
                labelBottom = labels[i + 1][j];
            }
            
            labelLeft = label;
            if (j > 0) {
                labelLeft = labels[i][j - 1];
            }
            
            labelRight = label;
            if (j < mat.cols - 1) {
                labelRight = labels[i][j + 1];
            }
            
            if (label != labelTop || label != labelBottom || label!= labelLeft || label != labelRight) {
                mat.at<cv::Vec3b>(i, j)[0] = 0;
                mat.at<cv::Vec3b>(i, j)[1] = 0;
                mat.at<cv::Vec3b>(i, j)[2] = 255;
            }
        }
    }
    return mat;
}

void MyCallbackForRegion(int region_tb_, void *userData)
{
     cout << "MyCallbackForContrast : Region = "<< region_tb_ << " " << regularization_tb << " " << minRegion_tb<<endl;
 }

void MyCallbackForRegularization(int regularization_tb_, void *userData)
{
     cout << "MyCallbackForContrast : Region = "<< region_tb << " " << regularization_tb_ << " " << minRegion_tb<<endl;
}

void MyCallbackForMinRegion(int minRegion_tb_, void *userData)
{
     cout << "MyCallbackForContrast : Region = "<< region_tb << " " << regularization_tb << " " << minRegion_tb_<<endl;
}

int main(int argc, char *argv[]) {
    int key = 0;
    // Read the Lenna image. The matrix 'mat' will have 3 8 bit channels
    // corresponding to BGR color space.
    mat_ori = cv::imread("dog.png", CV_LOAD_IMAGE_COLOR);
    //cv::Mat mat = cv::imread(argv[1], CV_LOAD_IMAGE_COLOR);
    namedWindow("My Window", 1);

    //Create track bar to change brightness
    createTrackbar("region", "My Window", &region_tb, 100, MyCallbackForRegion);

     //Create track bar to change contrast
    createTrackbar("regularization", "My Window", &regularization_tb, 10000, MyCallbackForRegularization);

    createTrackbar("minRegion", "My Window", &minRegion_tb, 100, MyCallbackForMinRegion);

    cv::Mat res = doSlic(mat_ori);    
    imshow("My Window", res);

     // Wait until user press some key
    while(1){
        char c = cvWaitKey(0);
        if( c == 27 ) {
            mat_ori = cv::imread("dog.png", CV_LOAD_IMAGE_COLOR);
            res = doSlic(mat_ori);    
            imshow("My Window", res);
        } else
        if (c == 32){
            break;
        } 

    }
    //cv::imwrite(argv[5], mat);
    cv::imwrite("res_3.png", res);
    
    return 0;
}
