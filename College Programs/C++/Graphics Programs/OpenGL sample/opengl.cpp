#include <GL\glut.h>
/*#include <gl/gl.h> */
#include <math.h>
#include <stdio.h>
#include <vector>
#include <list>
#include <algorithm>
#include "polygon.h"
#include <fstream>
#include <iostream>

using namespace std;

#define TRUE 1
#define FALSE 0
#define PI 3.1415926535


static int old_x, old_y;

 

GLfloat trans_z = -40.0;
GLfloat trans_y = 0.0;
GLfloat trans_x = 0.0;
 
GLfloat rot_y = 0.0;
GLfloat rot_x = 0.0;
GLfloat rot_z = 0.0;

GLfloat scale_x = 1.0;
GLfloat scale_y = 1.0;
GLfloat scale_z = 1.0;

GLboolean XY    = TRUE;
GLboolean DEPTH = TRUE;
GLboolean CULL  = FALSE;
GLboolean SCALEX = TRUE;
GLboolean LIGHTING = TRUE;


list<polygon> p;
list<polygon>::iterator i;
float xMax, xMin, yMax, yMin, zMax, zMin,xdiff, ydiff, zdiff; 
float centerx, centery;

/* Prototypes */
void doSquare(void);
void doCube(void);
void checkDepth(void);
void checkCulling(void);
void readInFile(char* filename);

/*////////////////////////////////////////////////////////////////////////////*/

       static GLint v[24]=

        {
                -5,  5,  5,     /*/ 0 */
                 5,  5,  5,     /*/ 1*/
                 5, -5,  5,     /*/ 2*/
                -5, -5,  5,     /*/ 3*/
                -5, -5, -5,     /*/ 4*/
                -5,  5, -5,     /*/ 5*/
                 5,  5, -5,     /*/ 6*/
                 5, -5, -5,     /*/ 7*/
        };      /*/ end v*/

 
        static GLfloat colors[6][3]=
        {
                {1.0, 0.0, 0.0},
                {0.0, 1.0, 0.0},
                {0.0, 0.0, 1.0},
                {0.0, 1.0, 1.0},
                {1.0, 0.0, 1.0},
                {1.0, 1.0, 0.0}
        };

 

        static GLuint indices[24] =

        {
                0, 1, 2, 3, /*/f*/
                1, 6, 7, 2, /*/l*/
                5, 0, 3, 4, /*/r*/
                5, 6, 1, 0, /*/t*/
                3, 2, 7, 4, /*/b*/
                6, 5, 4, 7  /*/bk*/
        };

 

        static GLfloat norms[6][3]=
        {
                {0,  0,  1},
                {1, 0,  0},
                {-1,  0,  0},
                {0,  1,  0},
                {0, -1,  0},
                {0,  0, -1}
        };
void setMaxMin(float x, float y, float z) {
	if (x > xMax) xMax = x;
	if (y > yMax) yMax = y;
	if (x < xMin) xMin = x;
	if (y < yMin) yMin = y;
	if (z < zMin) zMin = z;
	if (z > zMax) zMax = z;
	xdiff = (xMax-xMin)/2;
	ydiff = (yMax-yMin)/2;
	zdiff = (zMax-zMin)/2;
}

void doCube(void)

{
        int count = 0;
        int col = 0;
        int norm = 0;

        /*/glDrawElements (GL_QUADS, 24, GL_UNSIGNED_INT, indices);     // i think this ignores color arrays ?
        //glDrawArrays (GL_QUADS, 0, 3);        // haven't got a clue as to why ? */
        
        glBegin(GL_QUADS);
        for (count = 0; count < 24; count++)
        {
                col = (int)floor(count/4);
                norm = (int)floor(count/4);
                glNormal3fv (norms[norm]);
                glColor3fv (colors[col]);
                glArrayElement (indices[count]);
        }
        glEnd();
}

void doMyPolygon(void) {
polygon pDisplay;
vector<point>::iterator itPoint;
point ptCur;
	for (i = p.begin(); i != p.end(); i++) {
		pDisplay = (*i);
		glColor3f(pDisplay.red, pDisplay.green, pDisplay.blue);
		glBegin(GL_POLYGON);
		for (itPoint = pDisplay.points.begin(); itPoint != pDisplay.points.end(); itPoint++) {
			ptCur = (*itPoint);
			glNormal3f(ptCur.nx, ptCur.ny, ptCur.nz);
			glVertex3f(ptCur.x, ptCur.y, ptCur.z);
		}
		glEnd();
	}

}


void doSquare(void)
{
        glBegin(GL_POLYGON);
                glVertex2f(-5.0, 5.0);
                glVertex2f(5.0, 5.0);
                glVertex2f(5.0, -5.0);
                glVertex2f(-5.0, -5.0);
        glEnd();

		/*glBegin(GL_POLYGON);
                glVertex2f(-10.0, 10.0);
                glVertex2f(10.0, 10.0);
                glVertex2f(10.0, -10.0);
                glVertex2f(-10.0, -10.0);
        glEnd();*/
}


GLfloat position[] = { 0.0, 0.0, 10.0, 1.0 };
 

void init(void)
{
        glShadeModel(GL_SMOOTH);
        glClearColor(0.0, 0.0, 0.0, 1.0);
 

        /*/ SetUp the lighting conditions */
        glLightfv(GL_LIGHT0, GL_POSITION, position);
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        {
                //GLfloat mat[3] = { 0.9, 0.2, 0.2 };
				GLfloat mat[4] = { 1.0, 1.0, 1.0, 1.0};
                //mat[0] = 1.0; mat[1] = 1.0; mat[2] = 1.0;
                glMaterialfv(GL_FRONT, GL_SPECULAR, mat);
                glMaterialf(GL_FRONT, GL_SHININESS, 5.0);
        }
        glEnable(GL_COLOR_MATERIAL);
        glColorMaterial(GL_FRONT, GL_DIFFUSE);
        
		centerx = centery = 0;
		
		//glEnableClientState(GL_VERTEX_ARRAY);
        /*/glEnableClientState(GL_COLOR_ARRAY);
        //glEnableClientState(GL_NORMAL_ARRAY);
        //glNormalPointer(GL_INT, 3, norms); */
        //glVertexPointer(3, GL_INT, 0, v);
        /*/glColorPointer(3, GL_UNSIGNED_BYTE, 0, colors);
        //glColorPointer(3, GL_FLOAT, 0, colors); */
}

void checkDepth(void)
{
        if (DEPTH)
                glEnable(GL_DEPTH_TEST);
        else if (!DEPTH)
                glDisable(GL_DEPTH_TEST);
        glutPostRedisplay();
}

void checkLighting(void)
{
        if (LIGHTING)
                glEnable(GL_LIGHTING);
        else if (!LIGHTING)
                glDisable(GL_LIGHTING);
        glutPostRedisplay();
}

void checkCulling(void)
{
        if (CULL)
        {
                glEnable(GL_CULL_FACE);
                /*/glCullFace(GL_BACK); */
                glCullFace(GL_FRONT);
                glFrontFace(GL_CCW);
        }
        else if (!CULL)
                glDisable(GL_CULL_FACE);
        glutPostRedisplay();
}

void rotate (int rotX, int rotY)
{
        if (XY)
        {
                glRotatef(rotX, 1.0, 0.0, 0.0);
                glRotatef(rotY, 0.0, 1.0, 0.0);
        }
        else 
        {
                glRotatef(rotY, 0.0, 1.0, 0.0);
                glRotatef(rotX, 1.0, 0.0, 0.0);
        }
        glutPostRedisplay();
} 

void scaling (float scalingX, float scalingY)
{
        if (SCALEX)
                glScalef (scale_x, 1.0, 1.0);
        else if (!SCALEX)
                glScalef (1.0, scale_y, 1.0);
}

void display(void)
{
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();
        checkDepth();
        checkCulling();
        checkLighting();
        glColor3f(1.0, 1.0, 1.0);
        glPushMatrix();
                glTranslatef(0.0, 0.0, trans_z);
				glTranslatef(-xMin, 0.0, 0.0);
				glTranslatef(0.0, -yMin, 0.0);
				glTranslatef(0.0, 0.0, -zMin);
				glTranslatef(-xdiff, -ydiff, -zdiff);
				glScalef (5/xdiff, 5/ydiff, 5/zdiff);
				glTranslatef(centerx, centery, 0.0);
                rotate (rot_x, rot_y);
                /*/scaling (scale_x, scale_y);*/
                //doSquare(); 
                //doCube();
				doMyPolygon();
        glPopMatrix();
        glutSwapBuffers();
}

void reshape(int width, int height)
{
        GLfloat aspect = width/height;
        GLfloat left;
        GLfloat right;
        GLfloat top;
        GLfloat bottom;

        glViewport(0, 0, width, height);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        right = (width/100)/2;
        left = -right;
        top = (height/100)/2;
        bottom = -top;
		glFrustum(left, right, bottom, top, 10, 100);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
}


void motion(int x, int y)
{       
        trans_z += (old_y - y);
        old_y = y;
        if (trans_z <= -99.0)
                trans_z = -99.0;
        else if (trans_z >= -10.0)
                trans_z = -10.0;
        glutPostRedisplay();
}

void rotation(int x, int y)
{
        rot_x += (old_y - y);
        old_y = y;
        if (rot_x >= 360)

                rot_x -= 360.0;
        rot_y += (old_x - x);
        old_x = x;

        if (rot_y >= 360)
                rot_y -= 360.0;
        glutPostRedisplay();
}

void scale(int x, int y)
{
        scale_x += (old_y - y);
        old_y = y;
        if (scale_x >= 4.0)
                scale_x = 4.0;
        else if (scale_x <= 1.0)
                scale_x = 1.0;
        scale_y += (old_x - x);
        old_x = x;

        if (scale_y >= 4.0) 
               scale_y = 4.0;
        else if (scale_y <= 1.0)
                scale_y = 1.0;
}

void move(int x, int y)
{
	float floatX = x;
	float floatY = y;
	if (floatY > 240) floatY = 240 - (floatY - 240);
	else if (floatY < 240) floatY = 240 + (240 - floatY);
        /*scale_x += (old_y - y);
        old_y = y;
        if (scale_x >= 4.0)
                scale_x = 4.0;
        else if (scale_x <= 1.0)
                scale_x = 1.0;
        scale_y += (old_x - x);
        old_x = x;

        if (scale_y >= 4.0) 
               scale_y = 4.0;
        else if (scale_y <= 1.0)
                scale_y = 1.0;
		*/
	//cout << x << ", " << y << endl;
	centerx = (floatX/320.0 - 1.0) * 64;
	centery = (floatY/240.0 - 1.0) * 48;
}

void mouse(int button, int state, int x, int y)
{
        old_x = x;
        old_y = y;
        switch(button)
        {
        case GLUT_LEFT_BUTTON:
                if (state == GLUT_DOWN)
                {       
                        glutMotionFunc(motion);
                }
                if (state == GLUT_UP)
                        //glutPassiveMotionFunc(rotation);
						glutPassiveMotionFunc(NULL);
                break;
        case GLUT_RIGHT_BUTTON:
                if (state == GLUT_DOWN)
                {
                       glutMotionFunc(move);
                }
				if (state == GLUT_UP) {
                        centerx = 0.0;
						centery = 0.0;
				}
                break;
        default:
                break;
        }
        glutPostRedisplay();
}

 

void keys(unsigned char key, int x, int y)
{
        switch (key)
        {
        case 'O':
        case 'o':
                if (XY)
                {
                        XY = FALSE;
                /*      printf("\n XY = FALSE\n"); */
                }
                else if (!XY)
                {
                        XY = TRUE;
                /*      printf("\n XY = TRUE\n"); */
                }
                break;
        case 'Z':
        case 'z':
                if (DEPTH)
                {
                        DEPTH = FALSE;
                /*      printf("\n DEPTH = FALSE\n"); */
                }
                else if (!DEPTH)
                {
                        DEPTH = TRUE;
                /*      printf("\n DEPTH = TRUE\n"); */
                }
                break;
        case 'c':
        case 'C':
                if (CULL)
                {
                        CULL = FALSE;
                /*      printf("\n CULL = FALSE\n"); */
                }
                else if (!CULL)
                {
                        CULL = TRUE;
                /*      printf("\n CULL = TRUE\n"); */
                }
                break;
        case 'l':
        case 'L':
                if (LIGHTING)
                {
                        LIGHTING = FALSE;
                /*      printf("\n LIGHTING = FALSE\n"); */
                }
                else if (!LIGHTING)
                {
                        LIGHTING = TRUE;
               /*      printf("\n LIGHTING = TRUE\n"); */
                }
                break;
        case 's':
        case 'S':
                if (SCALEX)
                {
                        SCALEX = FALSE;
                /*      printf("\n SCALEX = FALSE\n"); */
                }
                else if (!SCALEX)
                {
                        SCALEX = TRUE;
                /*      printf("\n SCALEX = TRUE\n"); */
                }
                break;
        case 13:                        /*/ ascii for carriage return (RETURN) */
                rot_y = 0.0;
                rot_x = 0.0;
                trans_z = -40.0;
                scale_x = 1.0;
                scale_y = 1.0;
        /*      printf("\n Return pressed to RESET\n"); */
                glutPostRedisplay();
                break;
        case 27:
                printf("\n\n");
                /*exit 0;*/
        }
}


int main(int argc, char** argv)
{
		readInFile(argv[1]);
        glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
        glutInitWindowPosition(50, 50);
        glutInitWindowSize(640, 480);
        glutInit(&argc, argv);
        
		glutCreateWindow("A2 - Vertex Array Color Cube (Lighting)");
 
		init();
        glutDisplayFunc(display);
        glutReshapeFunc(reshape);

        glutMouseFunc(mouse);

        glutMotionFunc(NULL);
        glutPassiveMotionFunc(rotation);
 
        glutKeyboardFunc(keys);
        glutMainLoop();
        return 0;
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
				/*for (i=p.begin(); i != p.end(); i++) {
					//i->printVertices();
					i->transformPoints(xMax, xMin, yMax, yMin);
					//i->printVertices();
					i->createGlobalEdgeTable();
					//i->printGlobalEdgeTable();			
				}*/
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
				/*for (i=p.begin(); i != p.end(); i++) {
					//i->printVertices();
					i->transformPoints(xMax, xMin, yMax, yMin);
					//i->printVertices();
					i->createGlobalEdgeTable();
					//i->printGlobalEdgeTable();			
				}*/
		   }//end of else
	   }//end of test on file actually open	
}//end of readInFile
