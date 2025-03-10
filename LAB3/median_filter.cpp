// header inclusion
#include <stdio.h>
#include <opencv/cv.h>        //you may need to
#include <opencv/highgui.h>   //adjust import locations
#include <opencv/cxcore.h>    //depending on your machine setup

using namespace cv;
 
//sort the window using insertion sort
//insertion sort is best for this sorting
void insertionSort(int window[])
{
    int temp, i , j;
    for(i = 0; i < 9; i++){
        temp = window[i];
        for(j = i-1; j >= 0 && temp < window[j]; j--){
            window[j+1] = window[j];
        }
        window[j+1] = temp;
    }
}

void medianFilter(cv::Mat &input,cv::Mat &medianOutput){
    //create a sliding window of size 9
      int window[9]; 

    for(int y = 1; y < input.rows - 1; y++){
            for(int x = 1; x < input.cols - 1; x++){
 
                // Pick up window element
                window[0] = input.at<uchar>(y - 1 ,x - 1);
                window[1] = input.at<uchar>(y, x - 1);
                window[2] = input.at<uchar>(y + 1, x - 1);
                window[3] = input.at<uchar>(y - 1, x);
                window[4] = input.at<uchar>(y, x);
                window[5] = input.at<uchar>(y + 1, x);
                window[6] = input.at<uchar>(y - 1, x + 1);
                window[7] = input.at<uchar>(y, x + 1);
                window[8] = input.at<uchar>(y + 1, x + 1);
 
                // sort the window to find median
                insertionSort(window);
 
                // assign the median to centered element of the matrix
                medianOutput.at<uchar>(y,x) = window[4];
            }
        }
}
 
int main()
{
      Mat src, dst;
 
      // Load an image
      src = imread("car2.png", CV_LOAD_IMAGE_UNCHANGED);
 
      dst = src.clone();
        for(int y = 0; y < src.rows; y++){
            for(int x = 0; x < src.cols; x++){
                dst.at<uchar>(y,x) = 0;
            }
        }

        
      medianFilter(src,dst);
      medianFilter(dst,dst);
      medianFilter(dst,dst);
 
 
      namedWindow("final");
      imshow("final", dst);
 
      namedWindow("initial");
      imshow("initial", src);

      imwrite("fixed.jpg",dst);
 
      waitKey();
 
 
    return 0;
}