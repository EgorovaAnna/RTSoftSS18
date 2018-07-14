#include "SearchLines.h"
#include "ApproxLines.h"

class SearchLineOnField
{
	int **MaskField; //each point corresponds to the part of the image (1 - field, -1 - not field)
	int x, y; //dimentions of MaskField (how many pieces we are cutting on from frame)
	int dw, dh;
	int fieldFrom, fieldTo; //HSV hue parametres for searching field 
	SearchLines *sl;
	ApproxLines *al;
public:
	SearchLineOnField(int nx, int ny, int fF = 20, int fT = 110, SearchLines *nsl = new SearchLines(), ApproxLines *nal = new ApproxLines());
	//SearchLineOnField(int nx, int ny, int fF = 20, int fT = 110);
	std::vector<Vec4f> search(Mat frame);
private:
	int isField(Mat littleframe);
	void replaceMistakes(bool lineRedact = false);
	bool onField(Vec4f line);
};
