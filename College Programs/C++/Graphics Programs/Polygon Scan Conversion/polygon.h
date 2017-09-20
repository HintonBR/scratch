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
	polygon(int, int, int ,int);
	~polygon();
	void addVertice(int, int, int);
	void printVertices();
	vector<point> getVertices() {return points;};
	int getNumVertices() {return vertices;};
	int getRed(){return red;};
	int getBlue(){return blue;};
	int getGreen(){return green;};
	int getYMin() {return ymin;};
	int getYMax() {return ymax;};
	void setYMin(int y) {ymin = y;};
	void setYMax(int y) {ymax = y;};
	void createGlobalEdgeTable();
	void printGlobalEdgeTable();
	list<edge> getEdgeEntry(int y); 
private:
	int red;
	int green;
	int blue;
	int vertices;
	int ymin;
	int ymax;
	list<edge>*  gEdgeTable;
	vector<point> points;

};//end of class polygon

