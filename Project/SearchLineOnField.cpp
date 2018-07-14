#include "SearchLineOnField.h"

SearchLineOnField::SearchLineOnField(int nx, int ny, int fF, int fT, SearchLines *nsl, ApproxLines *nal)
{
	if (nx > 1 && ny > 1)
	{
		for (int i = 0; i < nx; i++)
			MaskField[i] = new int[ny];
		sl = nsl;
		al = nal;
		fieldFrom = fF;
		fieldTo = fT;
	}
}
/*SearchLineOnField::SearchLineOnField(int nx, int ny, int fF, int fT)
{
	if (nx > 1 && ny > 1)
		for (int i = 0; i < nx; i++)
			MaskField[i] = new int[ny];
	fieldFrom = fF;
	fieldTo = fT;
}*/
std::vector<Vec4f> SearchLineOnField::search(Mat frame)
{
	std::vector<Vec4f> lines, linesOnField;
	dw = frame.cols/y;
	dh = frame.rows/x;
	for (int i = 0; i < x; i++)
	{
		for (int j = 0; j < y; j++)
		{
			MaskField[i][j] = isField(frame(Range(i*dw, (i + 1)*dw), Range(j*dh, (j + 1)*dh)));
		}
	}
	replaceMistakes();
	replaceMistakes();
	replaceMistakes(true);
	replaceMistakes(true);
	lines = sl -> lines(frame);
	for (auto& line : lines)
		if (onField(line))
			linesOnField.push_back(line);
	return linesOnField;
	
}
int SearchLineOnField::isField(Mat littleframe)
{
	Mat hsv = littleframe;
	int sum = 0;
	std::vector<Mat> splitedHsv;
	cvtColor(hsv, hsv, COLOR_BGR2HSV);
	split(hsv, splitedHsv);
	for (int i = 0; i < hsv.cols; i++)
	{
		for (int j = 0; j < hsv.rows; j++)
		{
			if (static_cast<int>(splitedHsv[0].at<uchar>(i, j)) > fieldFrom && static_cast<int>(splitedHsv[0].at<uchar>(i, j)) < fieldTo)
				sum++;
		}
	}
	if (sum*100/littleframe.total() > 65)
		return 1;
	if (sum*100/littleframe.total() > 50)
		return 0;
	return -1;
}
void SearchLineOnField::replaceMistakes(bool lineRedact)
{
	int left = 0, right = 0;
	for (int i = 0; i < x - 1; i++)
	{
		for (int j = 0; j < y - 1; j++)
		{
			if (!lineRedact && MaskField[i][j] == 0)
				MaskField[i][j] = 1;
			if (MaskField[i][j] == 1)
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
			if (i != 0 && MaskField[i][j] != MaskField[i + 1][j] && MaskField[i][j] != MaskField[i - 1][j] && MaskField[i + 1][j] == MaskField[i - 1][j])
				MaskField[i][j] = MaskField[i - 1][j];
			if (j != 0 && MaskField[i][j] != MaskField[i][j + 1] && MaskField[i][j] != MaskField[i][j - 1] && MaskField[i][j + 1] == MaskField[i][j - 1])
				MaskField[i][j] = MaskField[i][j - 1];
		}
		if (lineRedact && right != 0 && right != left)
		{
			for (int j = left; j < right; j++)
				MaskField[i][j] = 1;	
		}
	}
}
bool SearchLineOnField::onField(Vec4f line)
{
	if ((MaskField[(int)line[1]/dh][(int)line[0]/dw] >= 0 || MaskField[(int)line[3]/dh][(int)line[2]/dw] >= 0))
		return true;
	return false;
}
