#include "opencv2/videoio.hpp"
#include "SearchLineOnField.h"
#include "opencv2/videostab/stabilizer.hpp"
#include "opencv2/videostab/ring_buffer.hpp"
#include <opencv2/calib3d.hpp>
#include "SearchBall.h"
#include "SearchGoal.h"

#define ALL_FIELD 0
#define GOAL_ON_FIELD 1
#define SOME_PART_OF_FIELD 2


class Field
{
	Mat field;
	Mat goal1, goal2;
	cv::Point ball;
	std::vector<Point> players;
	Ptr<BackgroundSubtractor> pMOG2;
	videostab::NullDeblurer wD;
	videostab::NullWobbleSuppressor nW;
	videostab::NullOutlierRejector nO;
	//Ptr<xphoto::WhiteBalancer> swb;
	SearchBall *sb;
	SearchGoal *sg;
	SearchLineOnField *lines;
	int timeOnSCreen;
	int typeOfImage;
public:
	Field(std::string file, int time = 50);
	void loadFieldImage(std::string file);
	void loadGoalImage(std::string file);
	void startGame(std::string videoFileName);
	void draw();
private:
	void read();
	void whatType(std::vector<Vec4f> linesBoundField);
	Point atField(std::vector<Vec4f> fieldLines, Rect object);
	Point pointOnLine(int xf1, int yf1, int xf2, int yf2, int x, int y);
	float distance(Point p1, Point p2);
	Point xyOnField(Point xyf[4], Rect object);
};
