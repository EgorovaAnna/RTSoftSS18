#include "opencv2/videoio.hpp"
#include "SearchLineOnField.h"
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
	SearchBall *sb;
	SearchGoal *sg;
	SearchLineOnField *lines;
	int timeOnSCreen;
	int typeOfImage;
public:
	Field(int time = 50);
	void loadFieldImage(std::string file);
	void loadGoalImage(std::string file);
	void startGame(std::string videoFileName);
	void draw();
private:
	void read();
	void whatType(std::vector<Vec4f> linesBoundField);
};
