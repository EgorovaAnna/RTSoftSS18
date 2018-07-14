#ifndef ApproxLines_h
#define ApproxLines_h

#include <iostream>
#include <vector>
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/bgsegm.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/xphoto/white_balance.hpp>

using namespace cv;

class ApproxLines
{
public:
	ApproxLines();
	std::vector<Vec4f> operator()(std::vector<Vec4f> lines);
	float distance(Vec4f line1, Vec4f line2);
};

#endif
