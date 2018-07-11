#include <iostream>
#include <string>
//#include <opencv2/xfeatures2d.hpp>
#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"

using namespace cv;

int isField(Mat &littleframe)
{
	//строим гистограмму и смотрим на пики
	int histSize[] = {16, 16, 16};
	int channels[] = {0, 1, 2};
	float arang[] = {0, 256};
	float brang[] = {0, 256};
	float crang[] = {0, 256};
	Mat hist;
	const float* ranges[] = {arang, brang, crang};
	calcHist(&littleframe, 1, channels, Mat(), hist, 3, &histSize[0], ranges, true, false);
}

int main()
{
	Mat frame1, frame2, grayframe;
	int keyboard = 0;
	Ptr<LineSegmentDetector> ls = createLineSegmentDetector(LSD_REFINE_STD);
	std::vector<Vec4f> lines_std;
	namedWindow("Frame");
	namedWindow("Frame2");
	frame1 = imread("goal.jpg");
	frame2 = imread("goal.jpg");
	//imshow("Frame", frame);
	cvtColor(frame1, grayframe, COLOR_BGR2GRAY);
	ls -> detect(grayframe, lines_std); // Detect the lines
	ls -> drawSegments(frame2, lines_std);
	imshow("Frame", frame2);
	
    keyboard = waitKey();
}
