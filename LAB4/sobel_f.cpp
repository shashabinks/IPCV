#include <stdio.h>
#include <opencv/cv.h>        //you may need to
#include <opencv/highgui.h>   //adjust import locations
#include <opencv/cxcore.h>    //depending on your machine setup

using namespace cv;

// Computes the x component of the gradient vector
// at a given point in a image.
// returns gradient in the x direction
int xGradient(Mat image, int x1, int y1){
    float sum;
    float Kernel[3][3] = {   //Using blur/sharp
                            {-1.0, 0.0, 1.0},
                            {-2.0, 0.0, 2.0},
                            {-1.0, 0.0, 1.0}
                           }; 

    for(int y=y1; y<y1+2; y++) {
        for(int x=x1; x<x1+2; x++) {
            sum = 0.0;
            for(int k = -1; k <= 1;k++){
                for(int j = -1; j <=1; j++){
                     sum = sum + Kernel[j+1][k+1]*image.at<uchar>(y - j, x - k);
            }
        }
    }
}

    return sum;
}
 
// Computes the y component of the gradient vector
// at a given point in a image
// returns gradient in the y direction
 
int yGradient(Mat image, int x1, int y1){
    float sum;

    float Kernel[3][3] = {   //Using blur/sharp
                            {-1.0,-2.0, -1.0},
                            {0.0, 0.0, 0.0},
                            {1.0, 2.0, 1.0}
                           }; 


    for(int y=y1; y<y1+2; y++) {
        for(int x=x1; x<x1+2; x++) {
            sum = 0.0;
            for(int k = -1; k <= 1;k++){
                for(int j = -1; j <=1; j++){
                     sum = sum + Kernel[j+1][k+1]*image.at<uchar>(y - j, x - k);
            }
        }
    }
}

    return sum;
}

void sobel(Mat src, cv::Mat &imx, cv::Mat &imy, cv::Mat &grad, cv::Mat &phse){
    imx = src.clone();
    imy = src.clone();
    grad = src.clone();
    phse = src.clone();

    int gx,gy,sumd,sumg;

    for(int y = 0; y < src.rows; y++){
            for(int x = 0; x < src.cols; x++){

                imx.at<uchar>(y,x) = 0.0;
                imy.at<uchar>(y,x) = 0.0;
                grad.at<uchar>(y,x) = 0.0;
                phse.at<uchar>(y,x) = 0.0;
            }
    }

    for(int y = 1; y < src.rows - 1; y++){
            for(int x = 1; x < src.cols - 1; x++){
                gx = xGradient(src, x, y);
                gy = yGradient(src, x, y);

                imx.at<uchar>(y,x) = gx;
                imy.at<uchar>(y,x) = gy;

                sumg = sqrt(abs(gx*gx) + abs(gy*gy));
                sumg = sumg > 255 ? 255:sumg;
                sumg = sumg < 0 ? 0 : sumg;
                grad.at<uchar>(y,x) = sumg;

                sumd = atan2(gy, gx) * 255;
                sumd = sumd > 255 ? 255:sumd;
                sumd = sumd < 0 ? 0 : sumd;
                phse.at<uchar>(y,x) = sumd;
            }
        }
}

int main(int argc, char** argv) { 

  // LOADING THE IMAGE
    char* imageName = argv[1];

    Mat image;
    image = imread( imageName, CV_LOAD_IMAGE_GRAYSCALE );

    if( argc != 2 || !image.data ) {
        printf( " No image data \n " );
        return -1;
    }

  Mat xdir,ydir,gdnt,phse;
  sobel(image,xdir,ydir,gdnt,phse);

  imwrite("x_test.jpg",xdir);
  imwrite("y_test.jpg",ydir);
  imwrite("mag_test.jpg",gdnt);
  imwrite("phase_test.jpg",phse);

  return 0;
}