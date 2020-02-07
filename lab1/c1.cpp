#include <stdio.h>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

int main() 
{

	Mat original, gray_image, binary, toBGR, toHSV, canny;
	original = imread("flowers.jpg");
	if(!original.data)
		{
            cout << "Could not open or find the image" << endl;
            return -1;
        }

	namedWindow("Original", WINDOW_NORMAL);
	resizeWindow("Original", 500, 500);
	imshow("Original", original);

	cvtColor(original, gray_image, CV_BGR2GRAY);
	namedWindow("Gray", WINDOW_NORMAL);
	resizeWindow("Gray", 500, 500);
	imshow("Gray", gray_image);

	threshold(gray_image, binary, 127, 255, THRESH_BINARY);
	namedWindow("Binary", WINDOW_NORMAL);
	resizeWindow("Binary", 500, 500);
	imshow("Binary", binary);

	threshold(original, toHSV, 127, 255, COLOR_BGR2HSV);
	namedWindow("HSV", WINDOW_NORMAL);
	resizeWindow("HSV", 500, 500);
	imshow("HSV", toHSV);

	threshold(original, toBGR, 127, 255, COLOR_BGR2RGB);
	namedWindow("RGB", WINDOW_NORMAL);
	resizeWindow("RGB", 500, 500);
	imshow("RGB", toBGR);

	Canny(gray_image, canny, 50, 150, 3);
	namedWindow("Canny", WINDOW_NORMAL);
	resizeWindow("Canny", 500, 500);
	imshow("Canny", canny);
	
	

	waitKey(0);
	return 0;
	
}