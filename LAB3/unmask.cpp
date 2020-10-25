// header inclusion
#include <stdio.h>
#include <opencv/cv.h>        //you may need to
#include <opencv/highgui.h>   //adjust import locations
#include <opencv/cxcore.h>    //depending on your machine setup

using namespace cv;

void GaussianBlur(
	cv::Mat &input, 
	int size,
	cv::Mat &blurredOutput);

int main( int argc, char** argv )
{

 // LOADING THE IMAGE
 char* imageName = argv[1];

 Mat image;
 image = imread( "car1.png", 1 );

 if( argc != 2 || !image.data )
 {
   printf( " No image data \n " );
   return -1;
 }

 // CONVERT COLOUR, BLUR AND SAVE
 Mat gray_image;
 cvtColor( image, gray_image, CV_BGR2GRAY );

 Mat carBlurred;
 GaussianBlur(gray_image,5,carBlurred);

 for ( int y = 0; y <gray_image.rows;y++){
	 for (int x = 0; x < gray_image.cols;x++){
		 int original = gray_image.at<uchar>(y , x);
		 int detail = original - carBlurred.at<uchar>(y,x);
		 int newPixel = (original + detail * 8);

		 carBlurred.at<uchar>(y , x) = newPixel;
	 }
 }

 double sigma=5, amount=1;
 Mat sharp1;
 Mat image3 = image;
 cvtColor(image3,image3,CV_BGR2GRAY);
 addWeighted(image3, 1 + amount, carBlurred, -amount, 1, sharp1);
 Mat lowContrastMask = abs(image3 - carBlurred) < 5; //experiment with values
 sharp1 = image3*(1+amount) + carBlurred*(-amount); //this is the same as addWeighted - is addWeightd obsolete??
 image3.copyTo(sharp1, lowContrastMask);

 image3 = image3 - lowContrastMask;

 Mat sharp2;
 Mat sharpening_kernel = (Mat_<double>(3, 3) << -1, -1, -1,  //convolute twice to get proper sharp image then apply unmask filter
        -1, 9, -1,
        -1, -1, -1);
filter2D(image, sharp2, -1, sharpening_kernel);

sharpening_kernel = (Mat_<double>(3, 3) << -1, -1, -1,
        -1, 9, -1,
        -1, -1, -1);
filter2D(sharp2, sharp2, -1, sharpening_kernel);

Mat result = sharp2; //- carBlurred;
 
 cvtColor(image3,image3,CV_GRAY2BGR);

 imwrite( "blur.jpg", carBlurred );
 
 /* this part works kinda
 Mat image1,image2;
 image1 = imread( "blur.jpg", CV_LOAD_IMAGE_GRAYSCALE);
 image2 = imread( "sharpened.jpg",CV_LOAD_IMAGE_GRAYSCALE);

 absdiff(image2,image1,result);
 
 cvtColor(result, result, CV_GRAY2RGB);
 */
 //imwrite("test2.jpg", sharp1);
 //imwrite("image3.jpg", image3);
 //imwrite("lowcon.jpg", lowContrastMask);

 //imwrite("sharper.jpg",result);

 return 0;
}

void GaussianBlur(cv::Mat &input, int size, cv::Mat &blurredOutput)
{
	// intialise the output using the input
	blurredOutput.create(input.size(), input.type());

	// create the Gaussian kernel in 1D 
	cv::Mat kX = cv::getGaussianKernel(size, -1);
	cv::Mat kY = cv::getGaussianKernel(size, -1);
	
	// make it 2D multiply one by the transpose of the other
	cv::Mat kernel = kX * kY.t();

	//CREATING A DIFFERENT IMAGE kernel WILL BE NEEDED
	//TO PERFORM OPERATIONS OTHER THAN GUASSIAN BLUR!!!

	// we need to create a padded version of the input
	// or there will be border effects
	int kernelRadiusX = ( kernel.size[0] - 1 ) / 2;
	int kernelRadiusY = ( kernel.size[1] - 1 ) / 2;

       // SET KERNEL VALUES
	for( int m = -kernelRadiusX; m <= kernelRadiusX; m++ ) {
	  for( int n = -kernelRadiusY; n <= kernelRadiusY; n++ )
           kernel.at<double>(m+ kernelRadiusX, n+ kernelRadiusY) = (double) 1.0/(size*size);

       }

	cv::Mat paddedInput;
	cv::copyMakeBorder( input, paddedInput, 
		kernelRadiusX, kernelRadiusX, kernelRadiusY, kernelRadiusY,
		cv::BORDER_REPLICATE );

	// now we can do the convoltion
	for ( int i = 0; i < input.rows; i++ )
	{	
		for( int j = 0; j < input.cols; j++ )
		{
			double sum = 0.0;
			for( int m = -kernelRadiusX; m <= kernelRadiusX; m++ )
			{
				for( int n = -kernelRadiusY; n <= kernelRadiusY; n++ )
				{
					// find the correct indices we are using
					int imagex = i + m + kernelRadiusX;
					int imagey = j + n + kernelRadiusY;
					int kernelx = m + kernelRadiusX;
					int kernely = n + kernelRadiusY;

					// get the values from the padded image and the kernel
					int imageval = ( int ) paddedInput.at<uchar>( imagex, imagey );
					double kernalval = kernel.at<double>( kernelx, kernely );

					// do the multiplication
					sum += imageval * kernalval;							
				}
			}
			// set the output value as the sum of the convolution
			blurredOutput.at<uchar>(i, j) = (uchar) sum;
		}
	}
}