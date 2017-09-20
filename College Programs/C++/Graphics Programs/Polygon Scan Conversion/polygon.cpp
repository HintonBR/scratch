#include "polygon.h"
#include <iostream>


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




polygon::polygon(int r, int g, int b,int v) {
	red = r;
	green = g;
	blue = b;
	vertices = v;
	ymin = -1;
	ymax = -1;
	
}

polygon::~polygon() {
}

void polygon::addVertice(int x, int y , int index) {
	point p;
	p.x = x;
	p.y = y;
	points.insert(points.begin() + index, p);
	if (y < ymin || ymin == -1) {
		ymin = y;
	}
	if (y > ymax || ymax == -1) {
		ymax = y;
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
	int ymx;
	int ymn;
	int xmn;
	int xmx;

	
	gEdgeTable = new list<edge>[(ymax - ymin) + 1];
	
	//Loop through vertices and create edges from two common vertices and set the edge in the global edge table
	for (int i = 0; i < (vertices - 1); i++) {
		a = points.at(i);
		b = points.at(i+1);
		if (a.y > b.y) {
			ymx = a.y;
			ymn = b.y;
			xmn = b.x;
			xmx = a.x;
		}
		else {
			ymx = b.y;
			ymn = a.y;
			xmn = a.x;
			xmx = b.x;
		}
		list<edge> n;
		list<edge> l = gEdgeTable[ymn-ymin];
		edge e;
		if (xmx != xmn) {
			e = edge(ymx, xmn, (float) (ymx - ymn)/(xmx - xmn));
		}
		else {
			e = edge(ymx, xmn, 0);
		}
		//Check if a list has already been created at that scanline - add to it or create a new one
		//Create edge with proper values, insert it into the list, sort it and then put it back in the global 
		//edge table
		if (l.empty() == false) {
			l.push_front(e);
			l.sort(edgesort());
			gEdgeTable[ymn-ymin] = l;
		}
		else {
			n.push_front(e);
			n.sort(edgesort());
			gEdgeTable[ymn-ymin] = n;
		}
	}//end of for loop

		//handles special case of edge connecting last point to first point
		a = points.at(vertices - 1);
		b = points.at(0);
		if (a.y > b.y) {
			ymx = a.y;
			ymn = b.y;
			xmn = b.x;
			xmx = a.x;
		}
		else {
			ymx = b.y;
			ymn = a.y;
			xmn = a.x;
			xmx = b.x;
		}
		list<edge> n;
		list<edge> l = gEdgeTable[ymn-ymin];
		edge e;
		if (xmx != xmn) {
			e = edge(ymx, xmn, (float)(ymx - ymn)/(xmx - xmn));
		}
		else {
			e = edge(ymx, xmn, 0);
		}
		//Check if a list has already been created at that scanline - add to it or create a new one
		//Create edge with proper values, insert it into the list, sort it and then put it back in the global 
		//edge table
		if (l.empty() == false) {
			l.push_front(e);
			l.sort(edgesort());
			gEdgeTable[ymn-ymin] = l;
		}
		else {
			n.push_front(e);
			n.sort(edgesort());
			gEdgeTable[ymn-ymin] = n;
		}
}//end of createGlobalEdgeTable method

void polygon::printGlobalEdgeTable() {
	list<edge> e;
	list<edge>::iterator it;
	edge ed;
	for (int i=0; i < ((ymax - ymin) + 1); i++) {
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
	return gEdgeTable[y - ymin];
}
