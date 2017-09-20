#include "polygon.h"
#include <iostream>



polygon::polygon(float r, float g, float b,int v) {
	red = r;
	green = g;
	blue = b;
	vertices = v;
}

polygon::~polygon() {
}

void polygon::addVertice(float x, float y , float z, float nx, float ny, float nz, int index) {
	point p;
	p.x = x;
	p.y = y;
	p.z = z;
	p.nx = nx;
	p.ny = ny;
	p.nz = nz;
	points.insert(points.begin() + index, p);

}

void polygon::printVertices() {
	for (int i = 0; i < vertices; i++) {
		cout << "x=" << points.at(i).x << " y=" << points.at(i).y << endl;
	}
}

