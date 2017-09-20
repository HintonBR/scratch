#pragma warning (disable:4786)
#include <vector>
#include <list>
#include <algorithm>
#include "point.h"

using namespace std;



class polygon {

public:
	polygon(){};
	polygon(float, float, float ,int);
	~polygon();
	void addVertice(float, float, float, float , float, float, int);
	void printVertices();
	vector<point> getVertices() {return points;};
	int getNumVertices() {return vertices;};
	float getRed(){return red;};
	float getBlue(){return blue;};
	float getGreen(){return green;};
		
	float red;
	float green;
	float blue;
	float vertices;
	vector<point> points;

};//end of class polygon

