#ifndef SearchGoal_h
#define SearchGoal_h

#include <iostream>
#include <vector>
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/bgsegm.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/xphoto/white_balance.hpp>

using namespace cv;

class SearchGoal
{
public:
	SearchGoal();
	std::vector<Vec4f> operator ()(std::vector<Vec4f> linesonfield);
};

#endif
