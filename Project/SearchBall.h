#ifndef SearchBall_h
#define SearchBall_h

#include <iostream>
#include <vector>
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/bgsegm.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/xphoto/white_balance.hpp>

using namespace cv;

class SearchBall
{
	cv::Point previousPosition;
public:
	SearchBall();
	cv::Point ball(Mat frame);//мне сюда нужны координаты углов поля или разметка
	cv::Point ballOnPlane(Mat frame, Mat plane);
	
};

#endif
