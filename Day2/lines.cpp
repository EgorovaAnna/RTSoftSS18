#include <iostream>
#include "opencv2/xfeatures2d.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/bgsegm.hpp>
#include <vector>

using namespace cv;

int main()
{
	Mat frame, grayframe;
	int keyboard;
	std::vector<Vec4f> lines_std;
	Ptr<LineSegmentDetector> ls = createLineSegmentDetector(LSD_REFINE_STD);
	namedWindow("Frame");
	VideoCapture capture("videoplayback.mp4");
   	if(!capture.isOpened())
	{
       		std::cout << "Unable to open video file " << '\n';
        	exit(EXIT_FAILURE);
    	}
    	while((char)keyboard != 27)
	{
        	if(!capture.read(frame)) 
		{
           		exit(EXIT_FAILURE);
        	}
		cvtColor( frame, grayframe, COLOR_BGR2GRAY );
		Canny(grayframe, grayframe, 50, 150, 3);
		ls -> detect(grayframe, lines_std); // Detect the lines
		ls -> drawSegments(frame, lines_std);
		imshow("Frame", grayframe);
       		keyboard = waitKey( 30 );
    	}
}
