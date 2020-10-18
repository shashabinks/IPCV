#include <opencv/cv.h>        //you may need to
#include <opencv/highgui.h>   //adjust import locations
#include <opencv/cxcore.h>    //depending on your machine setup
#include <vector>
#include <algorithm>

using namespace std;
using namespace cv;

void drawHist(const vector<int>& data, Mat3b& dst, int binSize = 3, int height = 0, int ref_value = -1)
{
    int max_value = *max_element(data.begin(), data.end());
    int rows = 0;
    int cols = 0;
    float scale = 1;
    if (height == 0) {
        rows = max_value + 10;
    }
    else {
        rows = height; 
        scale = float(height) / (max_value + 10);
    }
    cols = data.size() * binSize;
    dst = Mat3b(rows, cols, Vec3b(0, 0, 0));
    for (int i = 0; i < data.size(); ++i)
    {
        int h = rows - int(scale * data[i]);
        rectangle(dst, Point(i*binSize, h), Point((i + 1)*binSize - 1, rows), (i % 2) ? Scalar(0, 100, 255) : Scalar(0, 0, 255), CV_FILLED);
    }

    if (ref_value >= 0)
    {
        int h = rows - int(scale * ref_value);
        line(dst, Point(0, h), Point(cols, h), Scalar(255,0,0));
    }
}

int main()
{

    Mat1b src(100, 100);
    randu(src, Scalar(0), Scalar(255));

    int percent = 5; // percent % of pixel values are above a val
    int val;  // I need to find this value


    int n = src.rows * src.cols; // Total number of pixels
    int th = cvRound((100 - percent) / 100.f * n);  // Number of pixels below val

    // Histogram
    vector<int> hist(256, 0);
    for (int r = 0; r < src.rows; ++r) {
        for (int c = 0; c < src.cols; ++c) {
            hist[src(r, c)]++;
        }
    }

    // Cumulative histogram
    vector<int> cum = hist;
    for (int i = 1; i < hist.size(); ++i) {
        cum[i] = cum[i - 1] + hist[i];
    }

    // lower_bound returns an iterator pointing to the first element
    // that is not less than (i.e. greater or equal to) th.
    val =  distance(cum.begin(), lower_bound(cum.begin(), cum.end(), th));


    // Plot histograms
    Mat3b plotHist, plotCum;
    drawHist(hist, plotHist, 3, 300);
    drawHist(cum, plotCum, 3, 300, *lower_bound(cum.begin(), cum.end(), th));

    cout << "Value: " << val;

    imshow("Hist", plotHist);
    imshow("Cum", plotCum);
    waitKey();

    return 0;
}