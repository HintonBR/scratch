#include "polygon.h"
#include <iostream>
#define dvalue 4000

template<>
struct std::greater<edge>  : public binary_function<edge ,edge, bool> 
{
    bool operator()(const edge &x, const edge &y) const
    { 
	return (x.x < y.x); 
    };
};

// User-defined predicate function for sorting.
struct edgesort : public greater<edge> 
{
    bool operator()(const edge &pfi1, const edge &pfi2) const
    { 
	return pfi1.x < pfi2.x; 
    }
};




polygon::polygon(float r, float g, float b,int v) {
	red = r;
	green = g;
	blue = b;
	vertices = v;
	ymin = -1;
	ymax = -1;
	xmin = -1;
	xmax = -1;
	zmin = -1;
	zmax = -1;
	
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
	
	if (z < zmin || zmin == -1) {
		zmin = z;
	}
	if (z > zmax || zmax == -1) {
		zmax = z;
	}
}

void polygon::printVertices() {
	for (int i = 0; i < vertices; i++) {
		cout << "x=" << points.at(i).x << " y=" << points.at(i).y << endl;
	}
}

void polygon::createGlobalEdgeTable() {
	point a;
	point b;
	float ymx;
	float ymn;
	float xmn;
	float xmx;
	float zmn;
	float zmx;
	float nx, nxi, ny, nyi, nz, nzi, nxo, nyo, nzo,wx, wy,wz,wxo, wyo, wzo, ywx, ywy, ywz;

	gEdgeTable = new list<edge>[((int)(ymax - ymin))+1];
	
	//Loop through vertices and create edges from two common vertices and set the edge in the global edge table
	for (int i = 0; i < (vertices - 1); i++) {
		a = points.at(i);
		b = points.at(i+1);
		if (a.y > b.y) {
			zmx = a.z;
			ymx = a.y;
			xmx = a.x;
			ymn = b.y;
			xmn = b.x;
			zmn = b.z;
			nx = b.nx;
			ny = b.ny;
			nz = b.nz;
			nxo = a.nx;
			nyo = a.ny;
			nzo = a.nz;
			wx = b.wx;
			wy = b.wy;
			wz = b.wz;
			wxo = a.wx;
			wyo = a.wy;
			wzo = a.wz;
		}
		else {
			zmn = a.z;
			ymn = a.y;
			xmn = a.x;
			xmx = b.x;
			zmx = b.z;
			ymx = b.y;
			nx = a.nx;
			ny = a.ny;
			nz = a.nz;
			nxo = b.nx;
			nyo = b.ny;
			nzo = b.nz;
			wx = a.wx;
			wy = a.wy;
			wz = a.wz;
			wxo = b.wx;
			wyo = b.wy;
			wzo = b.wz;
		}
		list<edge> n;
		list<edge> l = gEdgeTable[(int) (ymn-ymin)];
		edge e;
		float yslope, zslope;

		if (xmx != xmn) yslope = (float) (ymx - ymn)/(xmx - xmn);
		else yslope = 0;
		if (ymx != ymn) {
			zslope = (float) (zmx-zmn)/(ymx - ymn);
			nxi = (float) (nxo - nx)/(ymx - ymn);
			nzi = (float) (nzo - nz)/(ymx - ymn);
			nyi = (float) (nyo - ny)/(ymx - ymn);
			ywx = (float) (wxo - wx)/(ymx - ymn);
			ywy = (float) (wyo - wy)/(ymx - ymn);
			ywz= (float) (wzo - wz)/(ymx - ymn);
		}
		else {
			zslope = 0;
			nxi = 0;
			nzi = 0;
			nyi = 0;
			ywx = 0;
			ywy = 0;
			ywz = 0;
		}
		
		e = edge(ymn, ymx, xmn,yslope , zmn,zslope, nx, nxi, ny, nyi, nz, nzi, wx, wy, wz, ywx, ywy, ywz);

		//Check if a list has already been created at that scanline - add to it or create a new one
		//Create edge with proper values, insert it into the list, sort it and then put it back in the global 
		//edge table
		if (l.empty() == false) {
			l.push_back(e);
			l.sort(edgesort());
			gEdgeTable[(int) (ymn-ymin)] = l;
		}
		else {
			n.push_back(e);
			n.sort(edgesort());
			gEdgeTable[(int) (ymn-ymin)] = n;
		}
	}//end of for loop

		//handles special case of edge connecting last point to first point
		a = points.at(vertices - 1);
		b = points.at(0);
		if (a.y > b.y) {
			zmx = a.z;
			ymx = a.y;
			ymn = b.y;
			xmn = b.x;
			zmn = b.z;
			xmx = a.x;
			nx = b.nx;
			ny = b.ny;
			nz = b.nz;
			nxo = a.nx;
			nyo = a.ny;
			nzo = a.nz;
			wx = b.wx;
			wy = b.wy;
			wz = b.wz;
			wxo = a.wx;
			wyo = a.wy;
			wzo = a.wz;
		}
		else {
			ymx = b.y;
			ymn = a.y;
			xmn = a.x;
			xmx = b.x;
			zmx = b.z;
			zmn = a.z;
			nx = a.nx;
			ny = a.ny;
			nz = a.nz;
			nxo = b.nx;
			nyo = b.ny;
			nzo = b.nz;
			wx = a.wx;
			wy = a.wy;
			wz = a.wz;
			wxo = b.wx;
			wyo = b.wy;
			wzo = b.wz;
		}
		list<edge> n;
		list<edge> l = gEdgeTable[(int) (ymn-ymin)];
		edge e;
		float yslope, zslope;

		if (xmx != xmn) yslope = (float) (ymx - ymn)/(xmx - xmn);
		else yslope = 0;
		if (ymx != ymn) {
			zslope = (float) (zmx-zmn)/(ymx - ymn);
			nxi = (float) (nxo - nx)/(ymx - ymn);
			nzi = (float) (nzo - nz)/(ymx - ymn);
			nyi = (float) (nyo - ny)/(ymx - ymn);
			ywx = (float) (wxo - wx)/(ymx - ymn);
			ywy = (float) (wyo - wy)/(ymx - ymn);
			ywz= (float) (wzo - wz)/(ymx - ymn);
		}
		else {
			zslope = 0;
			nxi = 0;
			nzi = 0;
			nyi = 0;
			ywx = 0;
			ywy = 0;
			ywz = 0;
		}
		
		e = edge(ymn, ymx, xmn,yslope , zmn,zslope, nx, nxi, ny, nyi, nz, nzi, wx, wy, wz, ywx, ywy, ywz);

		//Check if a list has already been created at that scanline - add to it or create a new one
		//Create edge with proper values, insert it into the list, sort it and then put it back in the global 
		//edge table
		if (l.empty() == false) {
			l.push_front(e);
			l.sort(edgesort());
			gEdgeTable[(int) (ymn-ymin)] = l;
		}
		else {
			n.push_front(e);
			n.sort(edgesort());
			gEdgeTable[(int) (ymn-ymin)] = n;
		}
}//end of createGlobalEdgeTable method

void polygon::printGlobalEdgeTable() {
	list<edge> e;
	list<edge>::iterator it;
	edge ed;
	for (int i=0; i < ((int)(ymax - ymin))+1; i++) {
			e = gEdgeTable[i];
			if (e.empty() == false) {
				cout << "For scanline " << i+ymin << endl;
				for (it=e.begin(); it != e.end(); it++) {
					ed = *it;
					cout << "(" << ed.ymax << ", " << ed.x << ", " << ed.xincrement << ") ";
				}
				cout << endl;
			}
	}
}

list<edge> polygon::getEdgeEntry(int y) {
	return gEdgeTable[(int) (y - ymin)];
}

void polygon::transformPoints(float xpmax, float xpmin,  float ypmax, float ypmin) {
	float x, y, z, xdiff, ydiff;
	
	point* a;
	xdiff =  (xpmax - xpmin)/2;
	ydiff = (ypmax - ypmin)/2;
	for (int i = 0; i < vertices; i++) {
		a = &(points.at(i));
		x = a->x;
		y = a->y;
		z = a->z;
		a->wx = x;
		a->wy = y;
		a->wz = z;
		if (x >= 0) x = (x - xdiff - xpmin)/xdiff;
		else x = (x + xdiff - xpmax)/xdiff;
		if (y >= 0) y = (y -  ydiff - ypmin)/ydiff;
		else y = (y + ydiff - ypmax)/ydiff;
		x = x *300;
		y = y *300;
		x += 400;
		y += 400;
		x = x/(1 - (z/dvalue));
		y = y/(1 - (z/dvalue));
		a->x = (int)x;
		a->y = (int)y;
		
		if (y < ymin || ymin == -1) {
			ymin = (int) y;
		}
		if (y > ymax || ymax == -1) {
			ymax = (int) y;
		}
		if (x < xmin || xmin == -1) {
			xmin = (int) x;
		}
		if (x > xmax || xmax == -1) {
			xmax = (int) x;
		}
	}
		

}
