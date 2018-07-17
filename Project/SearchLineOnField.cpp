#include "SearchLineOnField.h"

SearchLineOnField::SearchLineOnField(int nx, int ny, int fF, int fT, SearchLines *nsl, ApproxLines *nal)
{
	if (nx > 1 && ny > 1)
	{
		x = nx; y = ny;
		MaskField = new int*[nx];
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
			MaskField[i][j] = isField(frame(Range(j*dw, (j + 1)*dw), Range(i*dh, (i + 1)*dh)));
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
std::vector<Vec4f> SearchLineOnField::searchBoundingLines(Mat frame)
{
	dw = frame.cols/y;
	dh = frame.rows/x;
	xphoto::balanceWhite(frame, frame, 0);
	for (int i = 0; i < x; i++)
	{
		for (int j = 0; j < y; j++)
		{
			MaskField[i][j] = isField(frame(Range(i*dh, (i + 1)*dh), Range(j*dw, (j + 1)*dw)));
		}
	}
	replaceMistakes();
	replaceMistakes();
	replaceMistakes(true);
	replaceMistakes(true);
	for (int i = 0; i < x; i++)
	{
		for (int j = 0; j < y; j++)
		{
			std::cout << MaskField[i][j] + 1 << " ";
		}
		std::cout << std::endl;
	}
	return approxField(dh, dw);
}
std::vector<Vec4f> SearchLineOnField::approxField(int dh, int dw)
{
	std::vector<Vec4f> fieldlines;
	int x11, x12, x2, y11, y12, y2, x3, y3, count = 0;
	bool high = false;
	float k;
	for (int i = 0; i < x; i++)
	{
		for (int j = 0; j < y; j++)
		{
			if (MaskField[i][j] == 1)
			{
				x11 = j*dw;
				y11 = i*dh;
				x12 = j*dw;
				y12 = i*dh;
				count = 1;
				if (j != y - 1 && MaskField[i][j + 1] == 1)
				{
					while (j != y - 1 && MaskField[i][j + 1] == 1)
						j++;
					fieldlines.push_back(Vec4f((float)x11, (float)y11, (float)j*dw, (float)y11));
					x12 = j*dw;
					y12 = i*dh;
					high = true;
				}
				break;
			}
		}
		if (count == 1)
			break;
	}
	y2 = x*dh;
	for (int i = 0; i < x11/dw; i++)
	{
		for (int j = y11/dh; j < y2/dh; j++)
		{
			if (count == 1 && MaskField[j][i] == 1)
			{
				x2 = i*dw;
				y2 = j*dh;
				x3 = i*dw;
				y3 = j*dh;
				count++;
				continue;
			}
			if(count == 2 && MaskField[j][i] == 1 && i*dw != x3 && (float)(y11 - y3)/(x11 - x3) > (float)(j*dh - y3)/(i*dw - x3))
			{
				x3 = i*dw;
				y3 = j*dh;
			}
		}
	}
	if (high && (fieldlines[0])[2] -(fieldlines[0])[0] < (float)y*dw/4)
	{
		fieldlines.pop_back();
		fieldlines.push_back(Vec4f(x12, y12, x3, y3));
	}
	else
		fieldlines.push_back(Vec4f(x11, y11, x3, y3));
	if (x3 != x2)
		fieldlines.push_back(Vec4f(x3, y3, x2, y2));
	std::cout << "now all is ok\n";
	x11 = x2;
	y11 = y2;
	y2 = x*dh;
	for (int i = y - 1; i > x12/dw; i--)
	{
		for (int j = y12/dh; j < y2/dh; j++)
		{
			if (count == 2 && MaskField[j][i] == 1)
			{
				x2 = (i + 1)*dw;
				y2 = j*dh;
				x3 = (i + 1)*dw;
				y3 = j*dh;
				count++;
				continue;
			}
			if(count == 3 && MaskField[j][i] == 1 && i*dw != x12 && (float)(y3 - y12)/(x3 - x12) > (float)(j*dh - y12)/(i*dw - x12))
			{
				x3 = (i + 1)*dw;
				y3 = (j + 1)*dh;
			}
		}
	}
	if (y3 == (fieldlines[0])[3])
	{
		fieldlines.erase(fieldlines.begin());
		fieldlines.insert(fieldlines.begin(), Vec4f((fieldlines[0])[0], (fieldlines[0])[1], x3, y3));
	}
	else
		fieldlines.push_back(Vec4f(x12, y12, x3, y3));
	if (x3 != x2)
		fieldlines.push_back(Vec4f(x3, y3, x2, y2));
	x12 = x2;
	y12 = y2;
	if (x11 == 0)
	{		
		for (int i = x - 1; i > -1; i--)
		{
			if (MaskField[i][0] == 1)
			{
				fieldlines.push_back(Vec4f(x11, y11, 0, i*dh));
				y11 = i*dh;
				break;
			}
		}
	}
	if (x12/dw == y)
	{
		for (int i = x - 1; i > -1; i--)
		{
			if (MaskField[i][y - 1] == 1)
			{
				fieldlines.push_back(Vec4f(x12, y12, y*dw, i*dh));
				y12 = i*dh;
				break;
			}
		}
	}
	fieldlines.push_back(Vec4f(x11, y11, x12, y12));
	return fieldlines;
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
	if (sum*100/littleframe.total() > 59)
		return 1;
	if (sum*100/littleframe.total() > 54)
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
			/*if (!lineRedact && MaskField[i][j] == 0)
				MaskField[i][j] = 1;
			if (!lineRedact && MaskField[i][j + 1] == 0)
				MaskField[i][j + 1] = 1;*/
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
