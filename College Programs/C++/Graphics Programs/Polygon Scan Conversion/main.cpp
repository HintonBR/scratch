#include <GL/glut.h>
#include "polygon.h"
#include <fstream>
#include <iostream>
#include <list>
#include <vector>

list<polygon> p;
list<polygon>::iterator i;

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


void scanConvertLine(point p1, point p2);

void display(void) 
{ 
 polygon pDisplay;		
 int maxscanline, y, parity;
 float x;
 vector<point> points;
 int vertices;
 list<edge> aet, temp;
 list<edge>::iterator curedge;
 list<edge>::iterator nextedge;
 list<edge>::iterator it;
 list<list<edge>::iterator> delList;
 list<list<edge>::iterator>::iterator delListIt;

 glClear( GL_COLOR_BUFFER_BIT); 
 glColor3f(0.0, 0.0, 0.0); 
 glBegin(GL_POINTS); 
 for (i = p.begin(); i != p.end(); i++) {
	 pDisplay = *i;
	 vertices = pDisplay.getNumVertices();
	 glColor3f(pDisplay.getRed(), pDisplay.getGreen(), pDisplay.getBlue());
	 points = pDisplay.getVertices();
	 for (int j=0; j<(vertices-1); j++) {
		scanConvertLine(points.at(j), points.at(j+1));			
	 }
	 scanConvertLine(points.at(j), points.at(0));
 }


 //Do scan conversion of polygons
 for (i = p.begin(); i != p.end(); i++) {
	pDisplay = *i;
	aet.clear();
	maxscanline = pDisplay.getYMax();
	for (y=pDisplay.getYMin(); y <= maxscanline; y++) {
		parity = 0;
		edge e, t;

		//update and remove edges
		for (it = aet.begin(); it != aet.end(); it++) {
			e = *it;
			if (e.ymax <= y) {
				delList.push_front(it);
			}
			else { //update x value
				it->x += it->xincrement;
			}
				
		}

		//delete old edges
		for (delListIt = delList.begin(); delListIt != delList.end(); delListIt++) {
			aet.erase(*delListIt);
		}
		delList.clear();

		//add edges
		temp = pDisplay.getEdgeEntry(y); 
		aet.insert (aet.end(), temp.begin(), temp.end());
		aet.sort(edgesort());

		//get the first edge and rock and roll
		curedge = aet.begin();
		nextedge = aet.begin();
		nextedge++;
		while (nextedge != aet.end()) {
			if (y != (*curedge).ymax) {
				if (parity == 1) parity = 0;
				else parity = 1;
			}
			if (parity == 1) {
				x = (*curedge).x;
				t = (*nextedge);
				while (x < (*nextedge).x) {
					glColor3f(pDisplay.getRed(), pDisplay.getGreen(), pDisplay.getBlue());
					glVertex2f(x,y);
					x++;
				}
			}
			curedge++;
			nextedge++;
		}//end of while
	}//end of for
 }
 glEnd();
 glFlush(); 
} 
void processMouseEvent(int button, int state, int x, int y) {
	if (state == GLUT_UP) {
		/*if (waitingForPoint) {
			x1 = x;
			y1 = y;
			waitingForPoint = false;
			glutPostRedisplay();
		}
		else {
			x0 = x;
			y0 = y;
			waitingForPoint = true;
		}*/
		cout << "(" << x << ", " << y << ")" << endl;
	}
	glutPostRedisplay();
}
void processKeyBoardEvent(unsigned char key, int x, int y) {
	/*if (key = 13) {
		cout << "Enter x0" << endl;
		cin >> x0;
		cout << endl << "Enter y0" << endl;
		cin >> y0;
		cout << endl << "Enter x1" << endl;
		cin >> x1;
		cout << endl << "Enter y1" << endl;
		cin >> y1;
		cout << "A line will be drawn at (" << x0 << ", " << y0 << ") to (" << x1 << ", " << y1 << ")" << endl;
	}*/
	glutPostRedisplay();

}
void createOpenGLWindow(int argc, char **argv, float width, float height) {
	glutInit(&argc, argv); 
 glutInitDisplayMode ( GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH); 

 glutInitWindowPosition(100,100); 
 glutInitWindowSize(width, height); 
 glutCreateWindow ("square"); 
 

 glClearColor(1.0, 1.0, 1.0, 0.0); 
 glMatrixMode(GL_PROJECTION); 
 glLoadIdentity(); 
 glOrtho(0.0, width, height, 0.0, -1.0, 1.0); 
 glutMouseFunc(processMouseEvent);
 glutKeyboardFunc(processKeyBoardEvent);
 glutDisplayFunc(display); 
 glutMainLoop(); 
}

void readInFile(char* filename) {
	
	ifstream inFile;
	polygon newPoly;
	int red, green, blue, vertices, x, y;
	   
	
	//open file
	inFile.open(filename, ios::in);
	   if (!inFile) {
		   cout << "File could not be opened" << endl;
			exit(1);
	   }
	   while (inFile.eof() == 0) {
			inFile >> red;
			inFile >> green;
			inFile >> blue;
			inFile >> vertices;
			if (vertices != 0) {
				newPoly = polygon(red, green, blue, vertices);

				for (int i=0; i < vertices; i++) {
					inFile >> x;
					inFile >> y;
					newPoly.addVertice(x, y, i);
				}
				newPoly.createGlobalEdgeTable();
				newPoly.printGlobalEdgeTable();
				p.push_front(newPoly);
			}
			inFile.ignore(10000, '\n');
			red = green = blue = vertices = 0;
			
			
	   }	
}//end of readInFile


int main(int argc, char **argv) 
{ 
	int pause;
 readInFile(argv[1]);
 createOpenGLWindow(argc, argv, 800,800);
 for (i = p.begin(); i != p.end(); i++) {
	(*i).printVertices();	 
 }
 cin >> pause;
 return 0; 
}


void scanConvertLine(point p1, point p2) {
	int d, incrNE, incrSE, incrE, incrN, incrS;
	int x0, y0, x1, y1;
	x0 = p1.x;
	y0 = p1.y;
	x1 = p2.x;
	y1 = p2.y;

	if (x0 > x1) {	/*if first point is to the right of point two swap them*/
			int temp = x0;
			x0 = x1;
			x1 = temp;
			temp = y0;
			y0 = y1;
			y1 = temp;
	}
	
	int x = x0;
	int y = y0;
	/*Get the change in X and the change in Y*/
	int dx = x1 - x0;
	int dy = y1 - y0;

	
	glVertex2f(x,y);	/*The start pixel*/
	if (x == x1) {		/*The line is a vertical line so provide a special case*/
		if (y > y1) {
			int temp = y;
			y = y1;
			y1 = temp;
		}
		while (y <= y1) {
			glVertex2f(x,y);	/*Write out the pixel below the previous one*/
			y++;
		}
	}
	else if (y == y1) {		/*The line is a horizontal line so provide a special case*/
		if (x > x1) {
			int temp = x;
			x = x1;
			x1 = temp;
		}
		while (x <= x1) {
			glVertex2f(x,y);	/*Write out the pixel next to the previous one*/
			x++;
		}
	}
	else {
		if (dx >= dy && dy > 0) {	/*Slope is between 0 and 1*/
			d = 2 * dy - dx;		/*Intial value of d*/
			incrE = 2*dy;			/*Increment used for move to E*/
			incrNE = 2 *(dy - dx);	/*Increment used for move to NE*/
			while (x < x1) {
				if (d <=0) {			/*Choose E*/
					d += incrE;
					x++;
				}
				else {					/*Choose NE*/
					d += incrNE;
					x++;
					y++;
				}
				glVertex2f(x,y);	/*The selected pixel closest to the line*/
			}	/*while*/
		}
		else if (dx < dy && dy > 0) {	/*Slope is > 1*/
			d = 2*dx - dy;			/*Initial value of d*/
			incrN= 2*dx;			/*Increment used for move to N*/
			incrNE = 2 *(dx - dy );	/*Increment used for move to NE*/
			while (x < x1) {
				if (d <= 0) {			/*Choose N*/
					d += incrN;
					y++;
				}
				else {					/*Choose NE*/
					d += incrNE;
					x++;
					y++;
				}
				glVertex2f(x,y);	/*The selected pixel closest to the line*/
			}	/*while*/
		}
		else if (dx >= (-dy) && dy < 0) {	/*Slope is between 0 and -1*/
			d = 2*dy + dx;			/*Initial value of d*/
			incrE= 2*dy;			/*Increment used for move to E*/
			incrSE = 2 *(dy + dx );	/*Increment used for move to SE*/
			while (x < x1) {
				if (d > 0) {			/*Choose E*/
					d += incrE;
					x++;
				}
				else {					/*Choose SE*/
					d += incrSE;
					x++;
					y--;
				}
				glVertex2f(x,y);	/*The selected pixel closest to the line*/
			}	/*while*/
		}
		else if (dx < (-dy) && dy < 0) {	/*Slope < -1*/
			d = 2*dx + dy;			/*Initial value of d*/
			incrS= -2*dx;			/*Increment used for move to S*/
			incrSE = 2 *(-dx - dy );	/*Increment used for move to SE*/
			while (x < x1) {
				if (d > 0) {			/*Choose E*/
					d += incrS;
					y--;
				}
				else {					/*Choose SE*/
					d += incrSE;
					x++;
					y--;
				}
				glVertex2f(x,y);	/*The selected pixel closest to the line*/
			}	/*while*/
		}
	}
}


