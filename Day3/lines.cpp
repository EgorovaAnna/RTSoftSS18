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
#define GREEN_FROM 25
#define GREEN_TO 105

int isField(Mat littleframe)
{
	//строим гистограмму и смотрим на пики
	Mat hist, hsv = littleframe;
	cvtColor(hsv, hsv, COLOR_BGR2HSV);
	int h, sum = 0;
	std::vector<Mat> splitedHsv;
	split(hsv, splitedHsv);
	//balanceWhite(littleframe, littleframe, WHITE_BALANCE_SIMPLE);
	//xphoto::balanceWhite(littleframe, littleframe, 0);
	/*int histSize[] = {8, 8, 8}, sum = 0, green;
	int channels[] = {0, 1, 2};
	float arang[] = {0, 360};
	float brang[] = {0, 100};
	float crang[] = {0, 100};
	//lhist.zeros(Size(10, 10), CV_16UC1);
	const float* ranges[] = {arang, brang, crang};
	calcHist(&hsv, 1, channels, Mat(), hist, 3, &histSize[0], ranges);*/
	//for (int i = 3; i < 8; i++)
	//	sum += hist.at<int>(0, i, 0) + hist.at<int>(0, i, 1) + hist.at<int>(1, i, 0) + hist.at<int>(1, i, 1);// + hist.at<int>(0, i, 2) + hist.at<int>(2, i, 0) + hist.at<int>(2, i, 1) + hist.at<int>(1, i, 2);
	for (int i = 0; i < hsv.cols; i++)
	{
		for (int j = 0; j < hsv.rows; j++)
		{
			//std::cout << static_cast<int>(splitedHsv[0].at<uchar>(i, j)) << "  ";
			if (static_cast<int>(splitedHsv[0].at<uchar>(i, j)) > GREEN_FROM && static_cast<int>(splitedHsv[0].at<uchar>(i, j)) < GREEN_TO)
				sum++;
		}
		//std::cout << " \n ";
	}
		//std::cout << hist.at<int>(i, 7, j) << " ";
		//std::cout <<  "     ";
		//for (int j = 0; j < 8; j++)
		//	hist.at<int>(i, 7, j);
			//std::cout << hist.at<int>(i, 6, j) << " ";
		//std::cout << std::endl;
	//sum += hist.at<int>(8, 8, 8);
	//		for (int k = 0; k < 10; k++)
	//			lhist.at<int>(i, j) += hist.at<int>(j, i, 0);
	//namedWindow("hist");
	//Range range[3] = {Range(0, 10), Range(0, 10), Range(0, 1)};
	//imshow("hist", hist);
	//waitKey();
	if (sum*100/littleframe.total() > 70)
		return 1;
	if (sum*100/littleframe.total() > 55)
		return 0;
	return -1;
}
void replaceMistakes(int isField[TSIZE][TSIZE])
{
	for (int i = 1; i < TSIZE - 1; i++)
	{
		for (int j = 1; j < TSIZE - 1; j++)
		{
			if (isField[i][j] != isField[i + 1][j] && isField[i][j] != isField[i - 1][j])
				isField[i][j] = isField[i + 1][j];
			if (isField[i][j] != isField[i][j + 1] && isField[i][j] != isField[i][j - 1])
				isField[i][j] = isField[i][j + 1];
		}
	}
	
}
int main()
{
	Mat frame, frame2, grayframe;//, littleframes[TSIZE][TSIZE];
	int keyboard = 0, isfield[TSIZE][TSIZE], dh, dw;
	std::vector<std::vector<Point> > contours;
	std::vector<Vec4i> hierarchy;
	Ptr<LineSegmentDetector> ls = createLineSegmentDetector(LSD_REFINE_STD);
	//Ptr<xphoto::WhiteBalancer> wb = xphoto::createSimpleWB();
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
		cvtColor( frame, grayframe, COLOR_BGR2GRAY );
		Canny(grayframe, grayframe, 50, 150, 3); //+findContours?
		findContours( grayframe, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);
		drawContours( grayframe, contours, 3, Scalar(128,255,255), 3, LINE_AA, hierarchy, 3);
		ls -> detect(grayframe, lines_std);
		//ls -> drawSegments(frame, lines_std);
		//wb -> balanceWhite(frame, frame);
		xphoto::balanceWhite(frame, frame, 0);
		for (int i = 0; i < TSIZE; i++)
		{
			for (int j = 0; j < TSIZE; j++)
			{
				isfield[i][j] = isField(frame(Range(i*dw, (i + 1)*dw), Range(j*dh, (j + 1)*dh)));
				//std::cout << isfield[i][j] + 1 << " ";
			}
			//std::cout << std::endl;
		}
		std::cout << std::endl;
		std::cout << std::endl;
		replaceMistakes(isfield);
		replaceMistakes(isfield);
		for (int i = 0; i < TSIZE; i++)
		{
			for (int j = 0; j < TSIZE; j++)
			{
				std::cout << isfield[i][j] + 1 << " ";
			}
			std::cout << std::endl;
		}
		keyboard = waitKey(  );
		for (auto &line : lines_std)
		{
			std::cout << (int)line[0]/dw << " " << (int)line[1]/dh << " " << (int)line[2]/dw << " " << (int)line[3]/dh << " " << std::endl;
			std::cout << (int)line[0] << " " << (int)line[1] << " " << (int)line[2] << " " << (int)line[3] << " " << std::endl;
			if ((isfield[(int)line[1]/dh][(int)line[0]/dw] >= 0 && isfield[(int)line[3]/dh][(int)line[2]/dw] >= 0))
				lines.push_back(line);
		}
		ls -> drawSegments(frame2, lines);
		ls -> drawSegments(frame, lines_std);
		lines.clear();
		imshow("Frame", frame2);
		imshow("Frame2", frame);
		keyboard = waitKey( 30 );
    }
}
