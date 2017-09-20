#pragma warning (disable:4786)
#include <vector>
#include <list>
#include <algorithm>
#include "point.h"
#include "edge.h"

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
	float getYMin() {return ymin;};
	float getYMax() {return ymax;};
	void setYMin(int y) {ymin = y;};
	void setYMax(int y) {ymax = y;};
	void createGlobalEdgeTable();
	void printGlobalEdgeTable();
	list<edge> getEdgeEntry(int y); 
	void transformPoints(float xpmax, float xpmin,  float ypmax, float ypmin);

	float red;
	float green;
	float blue;
	float vertices;
	float ymin;
	float ymax;
	float xmin;
	float xmax;
	float zmin;
	float zmax;
	list<edge>*  gEdgeTable;
	vector<point> points;

};//end of class polygon

