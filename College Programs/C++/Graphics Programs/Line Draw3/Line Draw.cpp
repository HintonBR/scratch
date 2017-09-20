#include <stdio.h> 
#include <GL/glut.h>
#include <iostream.h> 

int x0 = 0;
int y0 = 0;
int x1 = 0;
int y1 = 0;
bool waitingForPoint = false;

void display(void) 
{ 
	int d, incrNE, incrSE, incrE, incrN, incrS;
	

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

	
 glClear( GL_COLOR_BUFFER_BIT); 
 glColor3f(0.0, 0.0, 0.0); 
 glBegin(GL_POINTS); 
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
  glEnd();
 glFlush(); 
} 
void processMouseEvent(int button, int state, int x, int y) {
	if (state == GLUT_UP) {
		if (waitingForPoint) {
			x1 = x;
			y1 = y;
			waitingForPoint = false;
			glutPostRedisplay();
		}
		else {
			x0 = x;
			y0 = y;
			waitingForPoint = true;
		}
	}
}
void processKeyBoardEvent(unsigned char key, int x, int y) {
	if (key = 13) {
		cout << "Enter x0" << endl;
		cin >> x0;
		cout << endl << "Enter y0" << endl;
		cin >> y0;
		cout << endl << "Enter x1" << endl;
		cin >> x1;
		cout << endl << "Enter y1" << endl;
		cin >> y1;
		cout << "A line will be drawn at (" << x0 << ", " << y0 << ") to (" << x1 << ", " << y1 << ")" << endl;
	}
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
int main(int argc, char **argv) 
{ 
 
 createOpenGLWindow(argc, argv, 300.0,300.0);
 
 return 0; 
}
