#include "Field.h"

Field::Field(std::string file, int time)
{
	sb = new SearchBall();
	sg = new SearchGoal();
	lines = new SearchLineOnField(40, 40); //magic number!
	timeOnSCreen = time;
	pMOG2 = createBackgroundSubtractorMOG2();
	videostab::NullDeblurer wD;
	videostab::NullWobbleSuppressor nW;
	videostab::NullOutlierRejector nO;
	loadFieldImage(file);
	typeOfImage = ALL_FIELD;
}
void Field::loadFieldImage(std::string file)
{
	field = imread(file);
}
void Field::loadGoalImage(std::string file)
{
	goal1 = imread(file);
	goal2 = goal1;
}
void Field::startGame(std::string videoFileName)
{
	int keyboard = 0;
	Mat frame, frame1, frame2, fgMaskMOG2;
	Point player;
	std::vector<std::vector<Point> > contours;
	std::vector<Rect> rects, notallrects;
	Ptr<LineSegmentDetector> ls = createLineSegmentDetector(LSD_REFINE_STD); //for drawing lines
	std::vector<Vec4f> line;
	VideoCapture cap(videoFileName.c_str());
	//VideoCapture cap("videoplayback.mp4");
	if(!cap.isOpened())
	{
    	std::cout << "Unable to open video file " << '\n';
      	exit(EXIT_FAILURE);
	}
	namedWindow("Frame");
	//namedWindow("Frame2");
	cap.read(frame);
	while((char)keyboard != 27)
	{
		if(!cap.read(frame)) 
			exit(EXIT_FAILURE);
		frame.copyTo(frame1);
		line = lines -> searchBoundingLines(frame1, true);
		frame.copyTo(frame2, lines -> getMask(frame.rows, frame.cols));
		//cvtColor(frame2, frame2, COLOR_BGR2GRAY );
		wD.deblur(3, frame2);
		//nW.suppress(300, frame2, frame2);
		xphoto::balanceWhite(frame2, frame2, 0);
		pMOG2 -> apply(frame2, fgMaskMOG2);
		findContours(fgMaskMOG2, contours, RETR_EXTERNAL, CHAIN_APPROX_TC89_L1 );
		rects.resize(contours.size());
		for(int i = 0; i < contours.size(); i++)
		{
			rects[i] = boundingRect(contours[i]);
			if(4*rects[i].width >= rects[i].height && 1.5*rects[i].width <= rects[i].height)
			{
				if (!(lines -> search(frame2(Range(rects[i].y, rects[i].y + rects[i].width), Range(rects[i].x, rects[i].x + rects[i].width)), true).empty()))
					continue;
				notallrects.push_back(rects[i]);
				player = atField(line, notallrects.back());
				rectangle(frame2, rects[i], cv::Scalar(250, 250, 250), 2);
			}
		}	
		ls -> drawSegments(frame, line);
		imshow("Frame", frame2);
		//imshow("Frame2", frame);
		//keyboard = waitKey(timeOnSCreen);
		waitKey();
		line.clear();
	}
	
}
Point Field::atField(std::vector<Vec4f> fieldLines, Rect object)
{
	int x[6], y[6], xf[4], yf[4], i;
	Point atfield, xyf[4];
	for (i = 0; i < 6; i++)
	{
		x[i] = fieldLines[0][1];
		y[i] = fieldLines[0][0];		
	}
	i = 0;
	x[0] = fieldLines[0][1];
	y[0] = fieldLines[0][0];
	if (fieldLines[0][0] < fieldLines[0][2])
	{
		x[1] = fieldLines[0][3];
		y[1] = fieldLines[0][2];
		x[2] = fieldLines[1][3];
		y[2] = fieldLines[1][2];
		x[3] = fieldLines[1][3];
		y[3] = fieldLines[1][2];
		i = 2;
		if (fieldLines[2][2] <= fieldLines[2][0])
		{
			x[2] = fieldLines[2][1];
			y[2] = fieldLines[2][0];
			x[3] = fieldLines[2][3];
			y[3] = fieldLines[2][2];
			i = 3;
			if (fieldLines[3][2] <= fieldLines[3][0])
			{
				x[2] = fieldLines[3][1];
				y[2] = fieldLines[3][0];
				x[3] = fieldLines[3][3];
				y[3] = fieldLines[3][2];
				i = 4;
			}
		}
	}
	else
	{
		x[0] = fieldLines[0][3];
		y[0] = fieldLines[0][2];
		x[1] = fieldLines[0][1];
		y[1] = fieldLines[0][0];
		x[2] = fieldLines[1][3];
		y[2] = fieldLines[1][2];
		x[3] = fieldLines[1][3];
		y[3] = fieldLines[1][2];
		i = 2;
		if (fieldLines[2][2] == fieldLines[2][0])
		{
			i = 3;
			x[3] = fieldLines[2][3];
			y[3] = fieldLines[2][2];
		}
	}
	if (fieldLines[i][2] < fieldLines[i + 1][2])
	{
		x[4] = fieldLines[i + 1][3];
		y[4] = fieldLines[i + 1][2];
		x[5] = fieldLines[i + 1][3];
		y[5] = fieldLines[i + 1][2];
		i++;
		if (i < fieldLines.size() - 1 && fieldLines[i + 1][2] == fieldLines[i + 1][0])
		{
			x[5] = fieldLines[i + 1][3];
			y[5] = fieldLines[i + 1][2];
		}
	}
	else
	{
		x[4] = fieldLines[i][3];
		y[4] = fieldLines[i][2];
		x[5] = fieldLines[i][3];
		y[5] = fieldLines[i][2];
		if (i < fieldLines.size() - 1 && fieldLines[i + 1][2] == fieldLines[i + 1][0])
		{
			x[5] = fieldLines[i + 1][3];
			y[5] = fieldLines[i + 1][2];
		}
	}
	if (typeOfImage == ALL_FIELD)
	{
		xf[0] = x[0];
		yf[0] = y[0];
		xf[1] = x[1];
		yf[1] = y[1];
		if (yf[2] != yf[3])
		{
			float k1 = (float)(x[2] - x[0])/(y[2] - y[0]);
			float b1 = -y[0]*k1 + x[0];
			float k2 = (float)(x[3] - x[5])/(y[3] - y[5]);
			float b2 = -y[5]*k2 + x[5];
			yf[2] = (b2 - b1)/(k1 - k2);
			xf[2] = k1*yf[2] + b1;
		}
		else
		{
			xf[1] = x[3];
			yf[1] = y[3];
		}
		if (yf[4] != yf[5])
		{
			float k1 = (float)(x[4] - x[1])/(y[4] - y[1]);
			float b1 = -y[1]*k1 + x[1];
			float k2 = (float)(x[5] - x[3])/(y[5] - y[3]);
			float b2 = -y[3]*k2 + x[3];
			yf[3] = (b2 - b1)/(k1 - k2);
			xf[3] = k1*yf[3] + b1;
		}
		else
		{
			xf[3] = x[5];
			yf[3] = y[5];
		}
		if (object.x > yf[1] || object.x < yf[0] || object.y < xf[1] || object.y > xf[2] || object.y > xf[3])
		{
			//std::cout << object.x  << " " << object.y << " :  " << xf[0] << " " << yf[0] << "; " << yf[1] << " " << xf[1] << "; " << yf[0] << " " << xf[2] << "; " << yf[3] << " " << xf[3] << "\n";
			return atfield;
		}
		for(i = 0; i < 4; i++)
		{
			xyf[i].x = xf[i];
			xyf[i].y = yf[i];
		}
		atfield = xyOnField(xyf, object);
		rectangle(field, Rect(atfield.y, atfield.x, object.width, object.width), cv::Scalar(250, 0, 0), 5);
		imshow("Fr", field);
		waitKey();
		
	}

	return atfield;
}
Point Field::xyOnField(Point xyf[4], Rect object)
{
	Point xy;
	Point xycentre, centre[2];
	bool ok = false;
	float alpha;
	centre[0].x = (float)(xyf[0].x + xyf[1].x)/2;
	centre[0].y = (float)(xyf[0].y + xyf[1].y)/2;
	centre[1].x = (float)(xyf[2].x + xyf[3].x)/2;
	centre[1].y = (float)(xyf[2].y + xyf[3].y)/2;
	xycentre = pointOnLine(centre[0].x, centre[0].y, centre[1].x, centre[1].y, object.y, object.x);
	alpha = (distance(xyf[2], xyf[3]) - distance(xyf[0], xyf[1]))/distance(centre[0], centre[1]);
	xy.x = xycentre.x*field.rows/(distance(centre[0], centre[1]));
	xy.y = distance(xycentre, Point(object.y, object.x))*field.rows/((distance(centre[0], centre[1]) - distance(centre[1], xycentre))*alpha + distance(xyf[0], xyf[1]));
	if (object.x < xycentre.y)
		xy.y = field.rows/2 - xy.y;
	else
		xy.y = field.rows/2 + xy.y;
	std::cout << xy.y << ";  " << xy.x << "\n";
	return xy;
}
float Field::distance(Point p1, Point p2)
{
	return pow(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2), 0.5);
}
Point Field::pointOnLine(int xf1, int yf1, int xf2, int yf2, int x, int y)
{
	Point xy;
	bool ok = false;
	float k1, b1, k2, b2;
	if (yf2 != yf1)
	{
		k1 = (float)(xf2 - xf1)/(yf2 - yf1);
		b1 = -yf1*k1 + xf1;
		k2 = -1/k1;
		b2 = x/k1 + yf1;
		xy.y = (b2 - b1)/(k1 - k2);
		xy.x = k2*xy.y + b2;
	}
	else
	{
		xy.y = yf1;
		xy.x = x;
	}
	return xy;
}
void Field::draw()
{
	std::cout << "point1\n";
	cv::namedWindow("Frame");
	std::cout << "point1.1\n";
	cv::imshow("Frame", 0);
	std::cout << "point1.2\n";
	waitKey(timeOnSCreen);
}
void Field::read()
{
}

