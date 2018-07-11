#include <iostream>
//#include "opencv2/xfeatures2d.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/bgsegm.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/xphoto/white_balance.hpp>
#include <vector>

using namespace cv;

#define TSIZE 40
#define GREEN_FROM 20
#define GREEN_TO 110

int isField(Mat littleframe)
{
	//turn image yo hsv format, count green pixels
	Mat hsv = littleframe;
	int sum = 0;
	std::vector<Mat> splitedHsv;
	cvtColor(hsv, hsv, COLOR_BGR2HSV);
	split(hsv, splitedHsv);
	for (int i = 0; i < hsv.cols; i++)
	{
		for (int j = 0; j < hsv.rows; j++)
		{
			if (static_cast<int>(splitedHsv[0].at<uchar>(i, j)) > GREEN_FROM && static_cast<int>(splitedHsv[0].at<uchar>(i, j)) < GREEN_TO)
				sum++;
		}
	}
	if (sum*100/littleframe.total() > 65)
		return 1;
	if (sum*100/littleframe.total() > 50)
		return 0;
	return -1;
}
void replaceMistakes(int isField[TSIZE][TSIZE], bool lineRedact = false)
{
	int left = 0, right = 0;
	for (int i = 0; i < TSIZE - 1; i++)
	{
		for (int j = 0; j < TSIZE - 1; j++)
		{
			if (!lineRedact && isField[i][j] == 0)
				isField[i][j] = 1;
			if (isField[i][j] == 1)
			{
				if (right == 0)
					left = j;
				else
					right = j;
			}
			else
			{
				if (right == 0)
					right = left;
			}
			if (i != 0 && isField[i][j] != isField[i + 1][j] && isField[i][j] != isField[i - 1][j] && isField[i + 1][j] == isField[i - 1][j])
				isField[i][j] = isField[i - 1][j];
			if (j != 0 && isField[i][j] != isField[i][j + 1] && isField[i][j] != isField[i][j - 1] && isField[i][j + 1] == isField[i][j - 1])
				isField[i][j] = isField[i][j - 1];
		}
		if (lineRedact && right != 0 && right != left)
		{
			for (int j = left; j < right; j++)
				isField[i][j] = 1;	
		}
	}
	
}
int main()
{
	Mat frame, frame1, frame2, grayframe;
	int keyboard = 0, isfield[TSIZE][TSIZE], dh, dw;
	std::vector<std::vector<Point> > contours;
	std::vector<Vec4i> hierarchy;
	Ptr<LineSegmentDetector> ls = createLineSegmentDetector(LSD_REFINE_STD);
	//Ptr<xphoto::WhiteBalancer> wb = xphoto::createSimpleWB(); //for opencv 4
	std::vector<Vec4f> lines_std, lines;
	namedWindow("Frame");
	namedWindow("Frame2");
	VideoCapture capture("videoplayback.mp4");
	capture.read(frame);
	dh = frame.cols/TSIZE;
	dw = frame.rows/TSIZE;
   	if(!capture.isOpened())
	{
    	std::cout << "Unable to open video file " << '\n';
      	exit(EXIT_FAILURE);
	}
    while((char)keyboard != 27)
	{
		if(!capture.read(frame)) 
			exit(EXIT_FAILURE);
		frame.copyTo(frame2);
		frame.copyTo(frame1);
		cvtColor(frame, grayframe, COLOR_BGR2GRAY );
		Canny(grayframe, grayframe, 30, 100, 3); //+findContours?
		findContours( grayframe, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);
		drawContours( grayframe, contours, 3, Scalar(128, 255 ,255), 3, LINE_AA, hierarchy, 3);
		ls -> detect(grayframe, lines_std);
		//wb -> balanceWhite(frame, frame); //for opencv 4
		xphoto::balanceWhite(frame, frame, 0);
		for (int i = 0; i < TSIZE; i++)
		{
			for (int j = 0; j < TSIZE; j++)
			{
				isfield[i][j] = isField(frame(Range(i*dw, (i + 1)*dw), Range(j*dh, (j + 1)*dh)));
			}
		}
		replaceMistakes(isfield);
		replaceMistakes(isfield);
		replaceMistakes(isfield, true);
		replaceMistakes(isfield, true);
		for (int i = 0; i < TSIZE; i++)
		{
			for (int j = 0; j < TSIZE; j++)
			{
				std::cout << isfield[i][j] + 1 << " ";
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
		keyboard = waitKey(  );
		for (auto &line : lines_std)
		{
			if ((isfield[(int)line[1]/dh][(int)line[0]/dw] >= 0 || isfield[(int)line[3]/dh][(int)line[2]/dw] >= 0))
				lines.push_back(line);
			else
			{
				std::cout << (int)line[0]/dw << " " << (int)line[1]/dh << " " << (int)line[2]/dw << " " << (int)line[3]/dh << " " << std::endl;
				std::cout << isfield[(int)line[1]/dh][(int)line[0]/dw] << " " << isfield[(int)line[3]/dh][(int)line[2]/dw] << " " << std::endl;
			}
		}
		ls -> drawSegments(frame2, lines); //lines on green field
		ls -> drawSegments(frame1, lines_std); // all lines
		lines.clear();
		imshow("Frame", frame2);
		imshow("Frame2", frame1);
		keyboard = waitKey( 30 );
    }
}
