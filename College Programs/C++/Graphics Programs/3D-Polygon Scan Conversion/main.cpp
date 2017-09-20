#include <glut.h>
#include "polygon.h"
#include <fstream>
#include <iostream>
#include <list>
#include <vector>
#include <math.h>

#define zbufferinit -999999999999

#define IAR 1
#define IAG 1
#define IAB 1
#define IPR 1
#define IPG 1
#define IPB 1
#define KA .8
#define KD .45
#define KS .35
#define LX 200
#define LY 200
#define LZ 1000

#define NCONSTANT 75
#define VX 0
#define VY 0
#define VZ 300 

list<polygon> p;
list<polygon>::iterator i;
float zbuffer[800][800];
float xMax, xMin, yMax, yMin, zMax, zMin;

void normal(float* x, float* y, float* z) {
	float a = (*x);
	float b = (*y);
	float c = (*z);
	float length = sqrt(a*a + b*b + c*c);
	*x = a/length;
	*y = b/length;
	*z = c/length;
}
float getNL(float wx, float wy, float wz, float nx, float ny, float nz) {
	float lx, ly, lz;
	lx = LX- wx;
	ly = LY - wy;
	lz = LZ - wz;
	normal(&lx, &ly, &lz);
	
	return nx * lx + ny * ly + nz * lz;
}
float getFATT(float wx, float wy, float wz) {
	float xdiff, ydiff, zdiff;
	xdiff = wx - LX;
	ydiff = wy - LY;
	zdiff = wz - LZ;
	return 1/(sqrt((xdiff * xdiff) + (ydiff * ydiff) + (zdiff * zdiff)));
}
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

bool isVisible(int x, int y, float z) {
	if (zbuffer[y][x] < z) {
		zbuffer[y][x] = z;
		return true;
	}
	else return false;
}

void display(void) 
{ 
 polygon pDisplay;		
 int y, parity, maxscanline;
 float x, z;
 vector<point> points;
 int vertices;
 list<edge> aet, temp;
 list<edge>::iterator curedge;
 list<edge>::iterator nextedge;
 list<edge>::iterator it;
 list<list<edge>::iterator> delList;
 list<list<edge>::iterator>::iterator delListIt;

 glBegin(GL_POINTS); 
 /*for (i = p.begin(); i != p.end(); i++) {
	 pDisplay = *i;
	 vertices = pDisplay.getNumVertices();
	 glColor3f(pDisplay.getRed(), pDisplay.getGreen(), pDisplay.getBlue());
	 points = pDisplay.getVertices();
	 for (int j=0; j<(vertices-1); j++) {
		scanConvertLine(points.at(j), points.at(j+1));			
	 }
	 scanConvertLine(points.at(j), points.at(0));
 }*/


 //Do scan conversion of polygons
 int count = 0;
 for (i = p.begin(); i != p.end(); i++) {
	 count++;
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
		if (temp.empty() == false ) {
		aet.insert (aet.end(), temp.begin(), temp.end());
		}
		aet.sort(edgesort());

		//get the first edge and rock and roll
		if (aet.empty() == false) {
		curedge = aet.begin();
		nextedge = aet.begin();
		nextedge++;
		while (nextedge != aet.end()) {
			if (y != (*curedge).ymax) {
				if (parity == 1) parity = 0;
				else parity = 1;
			}
			if (parity == 1) {
				x = (int)(*curedge).x;
				z = (*curedge).z;

				edge ce = (*curedge);
				edge ne = (*nextedge);
				float XW, YW, ZW, XWr, YWr, ZWr, Xinc, Yinc, Zinc, XN, YN, ZN, XNr, YNr, ZNr, XNinc, YNinc, ZNinc;
				
				XW = (y - ce.ymin)*ce.ywx + ce.wx;
				YW = (y - ce.ymin)*ce.ywy + ce.wy;
				ZW = (y - ce.ymin)*ce.ywz + ce.wz;
				XWr = (y - ne.ymin)*ne.ywx + ne.wx;
				YWr = (y - ne.ymin)*ne.ywy + ne.wy;
				ZWr = (y - ne.ymin)*ne.ywz + ne.wz;
	
				Xinc = (XWr - XW)/((ne.x) - (ce.x));
				Yinc = (YWr - YW)/((ne.x) - (ce.x));
				Zinc = (ZWr - ZW)/((ne.x) - (ce.x));
				//normals
				XN = (y - ce.ymin)*ce.nxincrement + ce.nx;
				YN = (y - ce.ymin)*ce.nyincrement + ce.ny;
				ZN = (y - ce.ymin)*ce.nzincrement + ce.nz;
				XNr = (y - ne.ymin)*ne.nxincrement + ne.nx;
				YNr = (y - ne.ymin)*ne.nyincrement + ne.ny;
				ZNr = (y - ne.ymin)*ne.nzincrement + ne.nz;
	
				XNinc = (XNr - XN)/((ne.x) - (ce.x));
				YNinc = (YNr - YN)/((ne.x) - (ce.x));
				ZNinc = (ZNr - ZN)/((ne.x) - (ce.x));

				

						

				while (x < ((*nextedge).x)) {
					
					if (isVisible(x, y, ZW)) {
						float tred, tgreen, tblue,nl;
						float rDiffuse, bDiffuse, gDiffuse, rPhong, bPhong, gPhong;
						float vx, vy, vz, xn, yn, zn;
						
						//normalize x,y,z normal vectors
						xn = XN;
						yn = YN;
						zn = ZN;

						float len = sqrt(xn*xn + yn*yn + zn*zn);
						xn = xn/len;
						yn = yn/len;
						zn = zn/len;
						
						//fatt = getFATT(XW, YW, ZW);
						//fatt = getFATT(ce.wx, ce.wy, ce.wz);
						
						//normalize vector to light point
						nl = getNL(XW, YW, ZW, xn, yn, zn);
						//nl = getNL(ce.wx, ce.wy, ce.wz, ce.nx, ce.ny, ce.nz);

						//normalize vector to view point
						vx = VX - XW;
						vy = VY - YW;
						vz = VZ - ZW;
						//vx = VX - ce.wx;
						//vy = VY - ce.wy;
						//vz = VZ - ce.wz;
						len = sqrt(vx*vx + vy*vy + vz*vz);
						vx = vx/len;
						vy = vy/len;
						vz = vz/len;
						//normal(&vx, &vy, &vz);

						rDiffuse = IPR * KD * pDisplay.getRed() * nl; 
						bDiffuse = IPB * KD * pDisplay.getBlue() * nl; 
						gDiffuse = IPG * KD * pDisplay.getGreen() * nl; 

						float Rr = 2 * xn * nl - vx;
						float Rg = 2 * yn * nl - vy;
						float Rb = 2 * zn * nl - vz;
						//float Rr = 2 * ce.nx * nl - vx;
						//float Rg = 2 * ce.ny * nl - vy;
						//float Rb = 2 * ce.nz * nl - vz;
						float RV = (Rr*vx + Rg*vy + Rb*vz);

						rPhong = KS * pow(RV,75);
						bPhong = KS * pow(RV,75);
						gPhong = KS*  pow(RV,75);
						//rPhong = 0;
						//bPhong = 0;
						//gPhong = 0;
												
						tred = IAR * KA * pDisplay.getRed() + rDiffuse + rPhong;
						tblue = IAB * KA * pDisplay.getBlue() + bDiffuse + bPhong;
						tgreen = IAG * KA * pDisplay.getGreen() + gDiffuse + gPhong;
						glColor3f(tred, tgreen, tblue);
						glVertex2f(x,y);
						
					}
					x++;

					XW = XW + Xinc;
					YW = YW + Yinc;
					ZW = ZW + Zinc;
					//normal increments
					XN = XN + XNinc;
					YN = YN + YNinc;
					ZN = ZN + ZNinc;
				}
			}
			curedge++;
			nextedge++;
		}//end of while
		}
	}//end of for
 }
 glEnd();
 glFlush(); 
 glClear (GL_COLOR_BUFFER_BIT);
} 

void processMouseEvent(int button, int state, int x, int y) {
	if (state == GLUT_UP) {
		cout << "(" << x << ", " << y << ")" << endl;
	}
	glutPostRedisplay();
}
void processKeyBoardEvent(unsigned char key, int x, int y) {
	glutPostRedisplay();
}
void createOpenGLWindow(int argc, char **argv, float width, float height) {
	glutInit(&argc, argv); 
 glutInitDisplayMode ( GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH); 

 glutInitWindowPosition(100,100); 
 glutInitWindowSize(width, height);
  glutCreateWindow ("3D Scan Conversion"); 
 

 glClearColor(0.0, 0.0, 0.0, 0.0); 
 glMatrixMode(GL_PROJECTION); 
 glLoadIdentity(); 
 glOrtho(0.0, width-1, 0.0, height-1, -1.0, 1.0); 
  glClear( GL_COLOR_BUFFER_BIT); 
 glColor3f(0.0, 0.0, 0.0); 
 glutMouseFunc(processMouseEvent);
 glutKeyboardFunc(processKeyBoardEvent);
 glutDisplayFunc(display); 
 glutMainLoop(); 
}

void setMaxMin(float x, float y, float z) {
	if (x > xMax) xMax = x;
	if (y > yMax) yMax = y;
	if (x < xMin) xMin = x;
	if (y < yMin) yMin = y;
	if (z < zMin) zMin = z;
	if (z > zMax) zMax = z;
}
void readInFile(char* filename) {
	
	ifstream inFile;
	polygon newPoly;
	int vertices, numPolys;
	float red, green, blue, x, y, z, nx, ny, nz;
	char format;   
	
	format = ' ';
	//open file
	inFile.open(filename, ios::in);
	   if (!inFile) {
		   cout << "File could not be opened" << endl;
			exit(1);
	   }
	   if (inFile.eof() == 0) {
		   while (format != 'n' && format != 'c') {
				inFile >> format;
		   }
		   if (format == 'n') {
				inFile >> numPolys;
				for (int k = 0; k < numPolys; k++) {
					inFile >> vertices;
					newPoly = polygon(1,1,1, vertices);
					for (int j = 0; j < vertices; j++) {
						inFile >> x;
						inFile >> y;
						inFile >> z;
						inFile >> nx;
						inFile >> ny;
						inFile >> nz;
						setMaxMin(x, y, z);
						newPoly.addVertice(x, y, z, nx, ny, nz, j);
					}
					p.push_back(newPoly);
				}
				for (i=p.begin(); i != p.end(); i++) {
					//i->printVertices();
					i->transformPoints(xMax, xMin, yMax, yMin);
					//i->printVertices();
					i->createGlobalEdgeTable();
					//i->printGlobalEdgeTable();			
				}
		   }
		   else {
			    list<polygon> tempList;
			    int polygroups;
				inFile >> polygroups;
				for (int k = 0; k < polygroups; k++) {
					tempList.clear();
					inFile >> numPolys;
					for (int v = 0; v < numPolys; v++) {
						inFile >> vertices;
						newPoly = polygon(1,1,1, vertices);
						for (int j = 0; j < vertices; j++) {
							inFile >> x;
							inFile >> y;
							inFile >> z;
							inFile >> nx;
							inFile >> ny;
							inFile >> nz;
							setMaxMin(x, y, z);
							newPoly.addVertice(x, y, z, nx, ny, nz, j);
						}
						tempList.push_front(newPoly);
					}
					inFile >> red;
					inFile >> green;
					inFile >> blue;
					for (i=tempList.begin(); i != tempList.end(); i++) {
						(*i).red = red;
						(*i).blue = blue;
						(*i).green = green;
						p.push_back((*i));
					}
				}
				for (i=p.begin(); i != p.end(); i++) {
					//i->printVertices();
					i->transformPoints(xMax, xMin, yMax, yMin);
					//i->printVertices();
					i->createGlobalEdgeTable();
					//i->printGlobalEdgeTable();			
				}
		   }//end of else
	   }//end of test on file actually open	
}//end of readInFile


int main(int argc, char **argv) 
{ 
	int pause;
 readInFile(argv[1]);
 for (int j= 0; j < 800; j++) {
	 for (int k=0; k < 800; k++) {
		zbuffer[j][k] = zbufferinit;
	 }
 }
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
	x0 = (int) p1.x;
	y0 = (int)  p1.y;
	x1 = (int)  p2.x;
	y1 = (int)  p2.y;

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


