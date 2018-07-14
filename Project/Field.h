#include "opencv2/videoio.hpp"
#include "SearchLineOnField.h"
#include "SearchBall.h"
#include "SearchGoal.h"

class Field
{
	Mat field;
	Mat goal1, goal2;
	cv::Point ball;
	SearchBall *sb;
	SearchGoal *sg;
	SearchLineOnField *lines;
	int timeOnSCreen;
public:
	Field(int time = 50);
	void loadFieldImage(std::string file);
	void loadGoalImage(std::string file);
	void startGame(std::string videoFileName);
private:
	void draw();
	void read();
};
