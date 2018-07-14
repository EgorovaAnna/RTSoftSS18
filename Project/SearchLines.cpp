#include "SearchLines.h"

SearchLines::SearchLines()
{
	ls = createLineSegmentDetector(LSD_REFINE_STD);
}
std::vector<Vec4f> SearchLines::lines(Mat frame)
{
	Mat grayframe;
	cvtColor(frame, grayframe, COLOR_BGR2GRAY );
	Canny(grayframe, grayframe, 30, 100, 3);
	findContours(grayframe, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);
	drawContours(grayframe, contours, 3, Scalar(128, 255 ,255), 3, LINE_AA, hierarchy, 3);
	ls -> detect(grayframe, allLines);
	return allLines;
}

