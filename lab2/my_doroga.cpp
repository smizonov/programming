#include "opencv2/opencv.hpp"
#include <iostream>
 
using namespace std;
using namespace cv;

int main()
{
	VideoCapture cap("video.mp4"); 
	if(!cap.isOpened())
		{
            cout << "Could not open or find the video" << endl;
            return -1;
        }

    int low_H = 0, low_S = 0, low_V = 170;
    int high_H = 256, high_S = 256, high_V = 256;

	Mat frame, Black_white_frame, frame_last;
	int x1_orig = 469, y1_orig = 517, x2_orig = 831, y2_orig = 517, x3_orig = 1092, y3_orig = 634, x4_orig = 276, y4_orig = 634;
	int x1_transform = 0, y1_transform = 0, x2_transform = 1280 , y2_transform = 0 , x3_transform = 1280, y3_transform = 720,x4_transform = 0, y4_transform = 720;

	Point2f original_video_coord[4];
	Point2f transform_video_coord[4];

	original_video_coord[0]=Point2f(x1_orig,y1_orig);
	original_video_coord[1]=Point2f(x2_orig,y2_orig);
	original_video_coord[2]=Point2f(x3_orig,y3_orig);
	original_video_coord[3]=Point2f(x4_orig,y4_orig);

	transform_video_coord[0]=Point2f(x1_transform,y1_transform);
	transform_video_coord[1]=Point2f(x2_transform,y2_transform);
	transform_video_coord[2]=Point2f(x3_transform,y3_transform);
	transform_video_coord[3]=Point2f(x4_transform,y4_transform);

	Mat trans_matrix;
	trans_matrix=getPerspectiveTransform(original_video_coord, transform_video_coord);
	while(1)
	{
		cap>>frame;
		Mat viev_above(720, 1280, CV_8UC1);

		warpPerspective(frame, viev_above, trans_matrix, viev_above.size());
		cvtColor(viev_above, Black_white_frame, CV_BGR2HSV);
		inRange(Black_white_frame, Scalar(low_H, low_S, low_V), Scalar(high_H, high_S, high_V), frame_last);//задает диапозон, по которым могу менять параметры H S и V
		


		line(frame, Point(x1_orig, y1_orig), Point(x2_orig, y2_orig), Scalar(0, 0, 255), 2);
		line(frame, Point(x2_orig, y2_orig), Point(x3_orig, y3_orig), Scalar(0, 0, 255), 2);
		line(frame, Point(x3_orig, y3_orig), Point(x4_orig, y4_orig), Scalar(0, 0, 255), 2);
		line(frame, Point(x4_orig, y4_orig), Point(x1_orig, y1_orig), Scalar(0, 0, 255), 2);

		namedWindow("original", WINDOW_NORMAL);
		resizeWindow("original", 850, 500);
		imshow("original", frame);

		namedWindow("birds eye view", WINDOW_NORMAL);
		resizeWindow("birds eye view" , 510, 300);
		imshow("birds eye view", frame_last);
		


		char key = (char)waitKey(25);
		if (key == 'q')
		{
			break;
		}


	}
	
}