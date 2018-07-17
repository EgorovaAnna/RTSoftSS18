#include "Field.h"

Field::Field(int time)
{
	sb = new SearchBall();
	sg = new SearchGoal();
	lines = new SearchLineOnField(40, 40); //magic number!
	timeOnSCreen = time;
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
	std::cout << "point1\n";
	int keyboard = 0;
	Mat frame, frame1;
	Ptr<LineSegmentDetector> ls = createLineSegmentDetector(LSD_REFINE_STD); //for drawing lines
	//VideoCapture cap(videoFileName.c_str());
	std::cout << "point1.1\n";
	VideoCapture cap("videoplayback.mp4");
	if(!cap.isOpened())
	{
    	std::cout << "Unable to open video file " << '\n';
      	exit(EXIT_FAILURE);
	}
	std::cout << "point1.2\n";
	namedWindow("Frame");
	cap.read(frame);
	while((char)keyboard != 27)
	{
		if(!cap.read(frame)) 
			exit(EXIT_FAILURE);
		frame.copyTo(frame1);
		std::cout << "point2.0\n";
		ls -> drawSegments(frame, lines -> searchBoundingLines(frame1));
		imshow("Frame", frame);
		//keyboard = waitKey(timeOnSCreen);
		waitKey();
	}
	
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

