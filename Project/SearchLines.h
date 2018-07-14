#ifndef SearchLines_h
#define SearchLines_h

#include <iostream>
#include <vector>
#include <string>
#include <opencv2/highgui.hpp>
#include <opencv2/bgsegm.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/xphoto/white_balance.hpp>

using namespace cv;

class SearchLines
{
	Ptr<LineSegmentDetector> ls;
	std::vector<std::vector<Point> > contours;
	std::vector<Vec4i> hierarchy;
	std::vector<Vec4f> allLines;
public:
	SearchLines();
	std::vector<Vec4f> lines(Mat frame);
};

#endif
