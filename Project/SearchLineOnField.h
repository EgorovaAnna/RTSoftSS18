#include "SearchLines.h"
#include "ApproxLines.h"

class SearchLineOnField
{
	int **MaskField; //each point corresponds to the part of the image (1 - field, -1 - not field)
	Mat  mask;
	int x, y; //dimentions of MaskField (how many pieces we are cutting on from frame)
	int dw, dh;
	int fieldFrom, fieldTo; //HSV hue parametres for searching field 
	SearchLines *sl;
	ApproxLines *al;
public:
	SearchLineOnField(int nx, int ny, int fF = 20, int fT = 110, SearchLines *nsl = new SearchLines(), ApproxLines *nal = new ApproxLines());
	std::vector<Vec4f> search(Mat frame, bool little);
	std::vector<Vec4f> search(Mat frame);
	std::vector<Vec4f> searchBoundingLines(Mat frame, bool approxmask = false);
	void approxMask(std::vector<Vec4f> boundlines, bool higher = false);
	Mat getMask(int nx, int ny);
private:
	int isField(Mat littleframe);
	void replaceMistakes(bool lineRedact = false);
	bool onField(Vec4f line);
	std::vector<Vec4f> approxField();
};
