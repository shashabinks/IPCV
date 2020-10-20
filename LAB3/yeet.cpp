/////////////////////////////////////////////////////////////////////////////
//
// COMS30121 - thr.cpp
// TOPIC: RGB explicit thresholding
//
// Getting-Started-File for OpenCV
// University of Bristol
//
/////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <opencv/cv.h>        //you may need to
#include <opencv/highgui.h>   //adjust import locations
#include <opencv/cxcore.h>    //depending on your machine setup

using namespace cv;

int main() { 

  // Read image from file
  Mat image = imread("car1.png", CV_LOAD_IMAGE_GRAYSCALE);
  Mat result;

  float Kernel[3][3] = {   //Using blur/sharp
                            {-1.0, -1.0, -1.0},
                            {-1.0, 9.0, -1.0},
                            {-1.0, -1.0, -1.0}
                           }; 

  result = image.clone();

  for(int y = 0; y < image.rows; y++){
            for(int x = 0; x < image.cols; x++){
                result.at<uchar>(y,x) = 0;
            }
  }

  // Threshold by looping through all pixels
  for(int y=0; y<image.rows; y++) {
   for(int x=0; x<image.cols; x++) {
     float sum = 0;

     for(int k = -1; k <= 1;k++){
        for(int j = -1; j <=1; j++){
            sum = sum + Kernel[j+1][k+1]*image.at<uchar>(y - j, x - k);
            }
     }
    result.at<uchar>(y,x) = sum;

   }
 } 

  namedWindow("Display window", CV_WINDOW_AUTOSIZE);
  namedWindow("Initial", CV_WINDOW_AUTOSIZE);

  imshow("Display window", result);
  imshow("Initial",image);

  cvtColor(result, result, CV_GRAY2RGB);
  imwrite("sharpened.jpg",result);

  waitKey(0);

  result.release();

  return 0;
}