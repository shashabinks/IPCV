#include <stdio.h>
#include <opencv/cv.h>        //you may need to
#include <opencv/highgui.h>   //adjust import locations
#include <opencv/cxcore.h>    //depending on your machine setup

using namespace cv;

void x_direction(Mat input,cv::Mat &output_x){

    output_x = input.clone();

    float Kernel[3][3] = {   //Using blur/sharp
                            {-1.0, 0.0, 1.0},
                            {-2.0, 0.0, 2.0},
                            {-1.0, 0.0, 1.0}
                           }; 

    for(int y = 0; y < input.rows; y++){
            for(int x = 0; x < input.cols; x++){
                output_x.at<uchar>(y,x) = 0;
        }
  }

  // Threshold by looping through all pixels
  for(int y=0; y<input.rows; y++) {
   for(int x=0; x<input.cols; x++) {
     float sum = 0;

     for(int k = -1; k <= 1;k++){
        for(int j = -1; j <=1; j++){
            sum = sum + Kernel[j+1][k+1]*input.at<uchar>(y - j, x - k);
            
            
            }
     }
    sum = sum > 255 ? 255:sum;
    sum = sum < 0 ? 0 : sum; 
    output_x.at<uchar>(y,x) = sum;

   }
 } 

  
}

void y_direction(Mat input, cv::Mat &output_y){
    


    output_y = input.clone();

    float Kernel[3][3] = {   //Using blur/sharp
                            {-1.0, -2.0, -1.0},
                            {0.0, 0.0, 0.0},
                            {1.0, 2.0, 1.0}
                           }; 

    for(int y = 0; y < input.rows; y++){
            for(int x = 0; x < input.cols; x++){
                output_y.at<uchar>(y,x) = 0;
        }
  }

  // Threshold by looping through all pixels
  for(int y=0; y<input.rows; y++) {
   for(int x=0; x<input.cols; x++) {
     float sum = 0;

     for(int k = -1; k <= 1;k++){
        for(int j = -1; j <=1; j++){
            sum = sum + Kernel[j+1][k+1]*input.at<uchar>(y - j, x - k);
            
            
            }
     }
    sum = sum > 255 ? 255:sum;
    sum = sum < 0 ? 0 : sum; 
    output_y.at<uchar>(y,x) = sum;

   }
 } 

 //imwrite("yy.jpg",output_y);

 
}

void gradient_im(Mat image1, Mat image2, cv::Mat &output_g){

    Mat dst;
    int sum,gx,gy;

    output_g = image1.clone();
    

    for(int y = 0; y < image1.rows; y++){
            for(int x = 0; x < image1.cols; x++){

                output_g.at<uchar>(y,x) = 0.0;
            }
    }

        for(int y = 1; y < image1.rows - 1; y++){
            for(int x = 1; x < image1.cols - 1; x++){
                gx = image1.at<uchar>(y , x );
                gy = image2.at<uchar>(y , x );

                 
                sum = sqrt((gx * gx) + (gy * gy));
                sum = sum > 255 ? 255:sum;
                sum = sum < 0 ? 0 : sum;
                output_g.at<uchar>(y,x) = sum;
            }
        }
}

void phase(Mat i1, Mat i2 , cv::Mat &output){
    int sum,gx,gy;
    output = i1.clone();

    for(int y = 0; y < i1.rows; y++){
            for(int x = 0; x < i1.cols; x++){

                output.at<uchar>(y,x) = 0.0;
            }
    }

        for(int y = 1; y < i1.rows - 1; y++){
            for(int x = 1; x < i1.cols - 1; x++){
                gx = i1.at<uchar>(y , x );
                gy = i2.at<uchar>(y , x );

                sum = atan2(gy, gx) * 255;
                sum = sum > 255 ? 255:sum;
                sum = sum < 0 ? 0 : sum;
                
                output.at<uchar>(y,x) = sum;
            }
        }

}



void sobel(Mat im1){
    Mat im2;
    Mat imy;
    Mat imx;
    Mat grad;
    Mat phse;

    y_direction(im1,imy);
    imwrite("ydir.jpg",imy);

    x_direction(im1,imx);
    imwrite("ydir2.jpg",imy);
    imwrite("xdir.jpg",imx);

    gradient_im(imx,imy,grad);
    imwrite("gimg.jpg",grad);

    phase(imx,imy,phse);
    imwrite("pimg.jpg",phse);
}

int main() { 

  // Read image from file
  Mat image = imread("Lena.png", CV_LOAD_IMAGE_GRAYSCALE);
  Mat result;

  float Kernel[3][3] = {   //Using blur/sharp
                            {-1.0, 0.0, 1.0},
                            {-2.0, 0.0, 2.0},
                            {-1.0, 0.0, 1.0}
                           }; 

  result = image.clone();

  sobel(image);
  return 0;
}