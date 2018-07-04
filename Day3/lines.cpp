#include <iostream>
//#include "opencv2/xfeatures2d.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/bgsegm.hpp>
#include <vector>

using namespace cv;

int isField(Mat &littleframe)
{
	//строим гистограмму и смотрим на пики
	int histSize[] = {10, 10, 10}, sum = 0;
	int channels[] = {0, 1, 2};
	float arang[] = {0, 256};
	float brang[] = {0, 256};
	float crang[] = {0, 256};
	Mat hist;
	const float* ranges[] = {arang, brang, crang};
	calcHist(&littleframe, 1, channels, Mat(), hist, 3, &histSize[0], ranges);
	for (int i = 3; i < 10; i++)
		sum += hist.at<int>(0, i, 0) + hist.at<int>(0, i, 1) + hist.at<int>(1, i, 0) + hist.at<int>(1, i, 1);
	if (sum*100/littleframe.total() > 70)
		return 1;
	if (sum*100/littleframe.total() > 50)
		return 0;
	return -1;
}

int main()
{
	Mat frame, frame2, grayframe, littleframes[10][10], dh, dw;
	int keyboard = 0, isfield[10][10];
	Ptr<LineSegmentDetector> ls = createLineSegmentDetector(LSD_REFINE_STD);
	std::vector<Vec4f> lines_std;
	namedWindow("Frame");
	VideoCapture capture("videoplayback.mp4");
	capture.read(frame);
	dh = (int)frame.cols/10;
	dw = (int)frame.rows/10;
   	if(!capture.isOpened())
	{
    	std::cout << "Unable to open video file " << '\n';
      	exit(EXIT_FAILURE);
	}
    while((char)keyboard != 27)
	{
		if(!capture.read(frame)) 
			exit(EXIT_FAILURE);
		cvtColor( frame, grayframe, COLOR_BGR2GRAY );
		Canny(grayframe, grayframe, 50, 150, 3);
		ls -> detect(grayframe, lines_std);
		//ls -> drawSegments(frame, lines_std);
		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 10; j++)
				std::cout << isfield[i][j] = isField(frame(Range((int)i*dw, (int)(i + 1)*dw), Range(j*dh, (j + 1)*dh))) << " ";
			std::cout << std::endl;
		}
		std::cout << std::endl;
		imshow("Frame", frame);
		keyboard = waitKey( 30 );
    }
}