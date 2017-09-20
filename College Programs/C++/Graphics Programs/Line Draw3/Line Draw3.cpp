#include <stdio.h> 
#include <GL/glut.h> 

void MidpointLine(int x0, int y0, int x1, int y1) {
	int dx = x1 - x0;
	int dy = y1 - y0;
	int d = 2 * dy - dx;		/*Intial value of d*/
	int incrE = 2*dy;			/*Increment used for move to E*/
	int incrNE = 2 *(dy - dx);	/*Increment used for move to NE*/
	int x = x0;
	int y = y0;
	glVertex2f(x/300,y/300);

	//WritePixel(x, y, value);	/*The start pixel*/

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
		glVertex2f(x/300,y/300);
		//WritePixel(x, y, value);	/*The selected pixel closest to the line*/
	}	/*while*/

}/*MidPointLine*/

void display(void) 
{ 
	int x0, y0, x1, y1;
	x0 = -50;
	y0 = 50;
	x1 = 150;
	y1 = 150;
	if (x1 < x0) {
		int temp = x0;
		x0 = x1;
		x1 = temp;
	}
	int dx = x1 - x0;
	int dy = y1 - y0;
	int d = 2 * dy - dx;		/*Intial value of d*/
	int incrE = 2*dy;			/*Increment used for move to E*/
	int incrNE = 2 *(dy - dx);	/*Increment used for move to NE*/
	int x = x0;
	int y = y0;
 glClear( GL_COLOR_BUFFER_BIT); 
 glColor3f(0.0, 0.0, 0.0); 
 glBegin(GL_POINTS); 
  //glVertex3f(2.0, 4.0, 0.0); 
  //glVertex3f(8.0, 4.0, 0.0); 
  //glVertex3f(8.0, 6.0, 0.0); 
  //glVertex3f(2.0, 6.0, 0.0); 
	//MidpointLine(5, 20, 100, 150);
	//glVertex2f(0.5,0.25);
	//glVertex2f(0.75,0.5);
	//glVertex2f(0.5,0.75);
	//glVertex2f(0.25,0.75);
	glVertex2f(x,y);

	//WritePixel(x, y, value);	/*The start pixel*/

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
		glVertex2f(x,y);
		//WritePixel(x, y, value);	/*The selected pixel closest to the line*/
	}	/*while*/
  glEnd();
 glFlush(); 
} 

int main(int argc, char **argv) 
{ 
 
 glutInit(&argc, argv); 
 glutInitDisplayMode ( GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH); 

 glutInitWindowPosition(100,100); 
 glutInitWindowSize(300,300); 
 glutCreateWindow ("square"); 

 glClearColor(1.0, 1.0, 1.0, 0.0); 
 glMatrixMode(GL_PROJECTION); 
 glLoadIdentity(); 
 glOrtho(0.0, 300.0, 300.0, 0.0, -1.0, 1.0); 
 
 glutDisplayFunc(display); 
 glutMainLoop(); 

 return 0; 
}
