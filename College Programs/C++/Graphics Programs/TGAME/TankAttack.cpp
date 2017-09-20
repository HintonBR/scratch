/*
 *		This Code Was Created By Jeff Molofee 2000
 *		A HUGE Thanks To Fredric Echols For Cleaning Up
 *		And Optimizing This Code, Making It More Flexible!
 *		If You've Found This Code Useful, Please Let Me Know.
 *		Visit My Site At nehe.gamedev.net
 */
#define SCREENWIDTH 800
#define SCREENHEIGHT 600
#define LAND "land3.bmp"
#define WALL "wall.bmp"
#define TANK "tank2.bmp"

#include <windows.h>		// Header File For Windows
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include <gl\glaux.h>		// Header File For The Glaux Library
#include <math.h>		// Header File For Windows Math Library		( ADD )
#include <stdio.h>		// Header File For Standard Input/Output	( ADD )
#include <stdarg.h>		// Header File For Variable Argument Routines	( ADD )

#include <vector>
#include <list>
#include <deque>
#include <algorithm>
#include "polygon.h"
#include <fstream>
#include <iostream>
#include "texture.h"
#include "TGame.h"
#include "tank.h"

using namespace std;

HDC			hDC=NULL;		// Private GDI Device Context
HGLRC		hRC=NULL;		// Permanent Rendering Context
HWND		hWnd=NULL;		// Holds Our Window Handle
HINSTANCE	hInstance;		// Holds The Instance Of The Application

bool	keys[256];			// Array Used For The Keyboard Routine
bool	active=TRUE;		// Window Active Flag Set To TRUE By Default
bool	fullscreen=TRUE;	// Fullscreen Flag Set To Fullscreen Mode By Default
bool	playing=true;

int winner = 0;

list<polygon> tankModel;
deque<Obstacle> obstacle1;
list<polygon> obstacle2;
list<polygon> obstacle3;
list<polygon>::iterator i;


TGame curGame;

GLuint     LandTexture, WallTexture, TankTexture;       /* Land texture object */
GLfloat rotangle = 0;
GLfloat LightAmbient[]= { 0.5f, 0.5f, 0.5f, 1.0f }; 				// Ambient Light Values ( NEW )
GLfloat LightDiffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f };				 // Diffuse Light Values ( NEW )
GLfloat LightPosition[]= { 0.0f, 0.0f, 100.0f, 1.0f };				 // Light Position ( NEW )

GLuint	base;			// Base Display List For The Font Set
GLfloat	cnt1=0;			// 1st Counter Used To Move Text & For Coloring
GLfloat	cnt2=0;			// 2nd Counter Used To Move Text & For Coloring




LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// Declaration For WndProc

void ResetGame() {
	playing = true;
	curGame.ResetTanks();
	rotangle = 0;
}
GLvoid BuildFont(GLvoid)								// Build Our Bitmap Font
{
	HFONT	font;										// Windows Font ID

	base = glGenLists(96);								// Storage For 96 Characters

	font = CreateFont(	-24,							// Height Of Font
						0,								// Width Of Font
						0,								// Angle Of Escapement
						0,								// Orientation Angle
						FW_BOLD,						// Font Weight
						FALSE,							// Italic
						FALSE,							// Underline
						FALSE,							// Strikeout
						ANSI_CHARSET,					// Character Set Identifier
						OUT_TT_PRECIS,					// Output Precision
						CLIP_DEFAULT_PRECIS,			// Clipping Precision
						ANTIALIASED_QUALITY,			// Output Quality
						FF_DONTCARE|DEFAULT_PITCH,		// Family And Pitch
						"Courier New");					// Font Name

	SelectObject(hDC, font);							// Selects The Font We Want

	wglUseFontBitmaps(hDC, 32, 96, base);				// Builds 96 Characters Starting At Character 32
}

GLvoid KillFont(GLvoid)									// Delete The Font
{
	glDeleteLists(base, 96);							// Delete All 96 Characters
}

GLvoid glPrint(const char *fmt, ...)					// Custom GL "Print" Routine
{
	char		text[256];								// Holds Our String
	va_list		ap;										// Pointer To List Of Arguments

	if (fmt == NULL)									// If There's No Text
		return;											// Do Nothing

	va_start(ap, fmt);									// Parses The String For Variables
	    vsprintf(text, fmt, ap);						// And Converts Symbols To Actual Numbers
	va_end(ap);											// Results Are Stored In Text

	glPushAttrib(GL_LIST_BIT);							// Pushes The Display List Bits
	glListBase(base - 32);								// Sets The Base Character to 32
	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);	// Draws The Display List Text
	glPopAttrib();										// Pops The Display List Bits
}



GLvoid ReSizeGLScene(GLsizei width, GLsizei height)		// Resize And Initialize The GL Window
{
	
	if (height==0)										// Prevent A Divide By Zero By
	{
		height=1;										// Making Height Equal One
	}

	glViewport(0,0,width,height);						// Reset The Current Viewport
	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	//Perspective calculation stuff
	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);
	
	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
}

int InitGL(GLvoid)										// All Setup For OpenGL Goes Here
{
	
	BuildFont();						// Build The Font

	//Create a texture object
	LandTexture = TextureLoad(LAND, GL_FALSE, GL_LINEAR,
                              GL_LINEAR, GL_REPEAT);
	WallTexture = TextureLoad(WALL, GL_FALSE, GL_LINEAR,
                              GL_LINEAR, GL_REPEAT);
	TankTexture = TextureLoad(TANK, GL_FALSE, GL_LINEAR,
                              GL_LINEAR, GL_REPEAT);
	
	

	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations

	
	//NeHe Lighting stuff
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);				// Setup The Ambient Light
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);				// Setup The Diffuse Light
	glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);			// Position The Light
	
	glEnable(GL_LIGHT1);										// Enable Light One
	glEnable(GL_LIGHTING);										// Enable Lighting

    //add Obstacles
	Obstacle Ob;
	  //first walls

		Ob = Obstacle(-.1,.5,.5,.2);
		curGame.addObstacle(Ob);
		Ob = Obstacle(0,0,.2,.205);
		curGame.addObstacle(Ob);
		Ob = Obstacle(0,-.7,1,.05);
		curGame.addObstacle(Ob);
		Ob = Obstacle(-.5,0,.05,1);
		curGame.addObstacle(Ob);
		Ob = Obstacle(.5,0,0.05,1);
		curGame.addObstacle(Ob);
	
		Ob = Obstacle(.95,0,0.1,2);
		curGame.addObstacle(Ob);
		Ob = Obstacle(0,.95,1.8,.1);
		curGame.addObstacle(Ob);
		Ob = Obstacle(0,-.95,1.8,.1);
		curGame.addObstacle(Ob);
		Ob = Obstacle(-.95,0,0.1,2);
		curGame.addObstacle(Ob);
	
		return TRUE;										// Initialization Went OK
		
}


void drawObstacles()
{
   glColor3f(0.44f,0.38f,0.33f);	//Brown
   glEnable(GL_TEXTURE_2D);

   glBindTexture(GL_TEXTURE_2D, WallTexture);

   for(int i=0;i<curGame.vObs.size();i++)
   {
	   Obstacle O = (Obstacle) curGame.vObs[i];
   
	   glBegin(GL_QUADS);
	
	  if(O.dir=='l')
	  {	
		glNormal3f( 0.0f, 0.0f, 1.0f);					// Normal Pointing Towards Viewer
		glTexCoord2f(1.0f, 1.0f); 
		glVertex3f (O.getE(), O.getN(),0.3);
		glTexCoord2f(0.0f, 1.0f); 
		glVertex3f (O.getW(), O.getN(), 0.3);
		glTexCoord2f(0.0f, 0.0f); 
		glVertex3f (O.getW(), O.getS(), 0.3);
		glTexCoord2f(1.0f, 0.0f); 
		glVertex3f (O.getE(), O.getS(), 0.3);

	
		//Inward Facing Quad
		glNormal3f( 1.0f, 0.0f, 0.0f);					// Normal Pointing Right
		glTexCoord2f(1.0f, 1.0f); 
		glVertex3f (O.getE(), O.getN(), 0.0);
		glTexCoord2f(0.0f, 1.0f); 
		glVertex3f (O.getE(), O.getN(), 0.3);
		glTexCoord2f(0.0f, 0.0f); 
		glVertex3f (O.getE(),O.getS(), 0.3);
		glTexCoord2f(1.0f, 0.0f); 
	    glVertex3f (O.getE(), O.getS(), 0.0);
		
		//Outward Facing Quad
		glNormal3f(-1.0f, 0.0f, 0.0f);					// Normal Pointing Left
		glTexCoord2f(1.0f, 1.0f); 
		glVertex3f (O.getW(), O.getN(), 0.3);
		glTexCoord2f(0.0f, 1.0f); 
		glVertex3f (O.getW(), O.getN(), 0.0);
		glTexCoord2f(0.0f, 0.0f); 
		glVertex3f (O.getW(), O.getS(), 0.0);
		glTexCoord2f(1.0f, 0.0f); 
	    glVertex3f (O.getW(), O.getS(), 0.3);

		//Top Side Quad
		glNormal3f( 0.0f, 1.0f, 0.0f);					// Normal Pointing Up
		glTexCoord2f(1.0f, 1.0f); 
		glVertex3f (O.getE(), O.getN(), 0.0);
		glTexCoord2f(0.0f, 1.0f); 
		glVertex3f (O.getW(), O.getN(), 0.0);
		glTexCoord2f(0.0f, 0.0f); 
		glVertex3f (O.getW(), O.getN(), 0.3);
		glTexCoord2f(1.0f, 0.0f); 
	    glVertex3f (O.getE(), O.getN(), 0.3);

		//Bottom Side Quad
		glNormal3f( 0.0f,-1.0f, 0.0f);					// Normal Pointing Down
		glTexCoord2f(1.0f, 1.0f); 
		glVertex3f (O.getE(), O.getS(), 0.3);
		glTexCoord2f(0.0f, 1.0f); 
		glVertex3f (O.getW(), O.getS(), 0.3);
		glTexCoord2f(0.0f, 0.0f); 
		glVertex3f (O.getW(), O.getS(), 0.0);
		glTexCoord2f(1.0f, 0.0f); 
	    glVertex3f (O.getE(), O.getS(), 0.0);

	  }
	  else if(O.dir=='r')
	  {	
		glNormal3f( 0.0f, 0.0f, 1.0f);					// Normal Pointing Towards Viewer
		glTexCoord2f(1.0f, 1.0f); 
		glVertex3f (O.getE(), O.getN(),0.3);
		glTexCoord2f(0.0f, 1.0f); 
		glVertex3f (O.getW(), O.getN(), 0.3);
		glTexCoord2f(0.0f, 0.0f); 
		glVertex3f (O.getW(), O.getS(), 0.3);
		glTexCoord2f(1.0f, 0.0f); 
		glVertex3f (O.getE(), O.getS(), 0.3);

	
		//Inward Facing Quad
		glNormal3f( 1.0f, 0.0f, 0.0f);					// Normal Pointing left
		glTexCoord2f(1.0f, 1.0f); 
		glVertex3f (O.getE(), O.getN(), 0.0);
		glTexCoord2f(0.0f, 1.0f); 
		glVertex3f (O.getE(), O.getN(), 0.3);
		glTexCoord2f(0.0f, 0.0f); 
		glVertex3f (O.getE(),O.getS(), 0.3);
		glTexCoord2f(1.0f, 0.0f); 
	    glVertex3f (O.getE(), O.getS(), 0.0);
		
		//Outward Facing Quad
		glNormal3f(-1.0f, 0.0f, 0.0f);					// Normal Pointing right
		glTexCoord2f(1.0f, 1.0f); 
		glVertex3f (O.getW(), O.getN(), 0.3);
		glTexCoord2f(0.0f, 1.0f); 
		glVertex3f (O.getW(), O.getN(), 0.0);
		glTexCoord2f(0.0f, 0.0f); 
		glVertex3f (O.getW(), O.getS(), 0.0);
		glTexCoord2f(1.0f, 0.0f); 
	    glVertex3f (O.getW(), O.getS(), 0.3);

		//Top Side Quad
		glNormal3f( 0.0f, 1.0f, 0.0f);					// Normal Pointing Up
		glTexCoord2f(1.0f, 1.0f); 
		glVertex3f (O.getE(), O.getN(), 0.0);
		glTexCoord2f(0.0f, 1.0f); 
		glVertex3f (O.getW(), O.getN(), 0.0);
		glTexCoord2f(0.0f, 0.0f); 
		glVertex3f (O.getW(), O.getN(), 0.3);
		glTexCoord2f(1.0f, 0.0f); 
	    glVertex3f (O.getE(), O.getN(), 0.3);

		//Bottom Side Quad
		glNormal3f( 0.0f,-1.0f, 0.0f);					// Normal Pointing Down
		glTexCoord2f(1.0f, 1.0f); 
		glVertex3f (O.getE(), O.getS(), 0.3);
		glTexCoord2f(0.0f, 1.0f); 
		glVertex3f (O.getW(), O.getS(), 0.3);
		glTexCoord2f(0.0f, 0.0f); 
		glVertex3f (O.getW(), O.getS(), 0.0);
		glTexCoord2f(1.0f, 0.0f); 
	    glVertex3f (O.getE(), O.getS(), 0.0);

	  }
	  else if(O.dir=='s')//bottom
	  {
		glNormal3f( 0.0f, 0.0f, 1.0f);					// Normal Pointing Towards Viewer
		glTexCoord2f(1.0f, 1.0f); 
		glVertex3f (O.getE(), O.getN(),0.3);
		glTexCoord2f(0.0f, 1.0f); 
		glVertex3f (O.getW(), O.getN(), 0.3);
		glTexCoord2f(0.0f, 0.0f); 
		glVertex3f (O.getW(), O.getS(), 0.3);
		glTexCoord2f(1.0f, 0.0f); 
	    glVertex3f (O.getE(), O.getS(), 0.3);

		//Inward Facing Quad
		glNormal3f( 0.0f, 1.0f, 0.0f);					// Normal Pointing Up
		glTexCoord2f(1.0f, 1.0f); 
		glVertex3f (O.getE(), O.getN(), 0.0);
		glTexCoord2f(0.0f, 1.0f); 
		glVertex3f (O.getW(), O.getN(), 0.0);
		glTexCoord2f(0.0f, 0.0f); 
		glVertex3f (O.getW(), O.getN(), 0.3);
		glTexCoord2f(1.0f, 0.0f); 
	    glVertex3f (O.getE(), O.getN(), 0.3);
		
		//Outward Facing Quad
		glNormal3f( 0.0f,-1.0f, 0.0f);					// Normal Pointing Down
		glTexCoord2f(1.0f, 1.0f); 
		glVertex3f (O.getE(), O.getS(), 0.3);
		glTexCoord2f(0.0f, 1.0f); 
		glVertex3f (O.getW(), O.getS(), 0.3);
		glTexCoord2f(0.0f, 0.0f); 
		glVertex3f (O.getW(), O.getS(), 0.0);
		glTexCoord2f(1.0f, 0.0f); 
	    glVertex3f (O.getE(), O.getS(), 0.0);

		//Left Facing Quad
		glNormal3f( -1.0f, 0.0f, 0.0f);					// Normal Pointing Left
		glTexCoord2f(1.0f, 1.0f); 
		glVertex3f (O.getW(), O.getN(), 0.3);
		glTexCoord2f(0.0f, 1.0f); 
		glVertex3f (O.getW(), O.getN(), 0.0);
		glTexCoord2f(0.0f, 0.0f); 
		glVertex3f (O.getW(),O.getS(), 0.0);
		glTexCoord2f(1.0f, 0.0f); 
	    glVertex3f (O.getW(), O.getS(), 0.3);
		
		//Right Facing Quad
		glNormal3f(1.0f, 0.0f, 0.0f);					// Normal Pointing Right
		glTexCoord2f(1.0f, 1.0f); 
		glVertex3f (O.getE(), O.getN(), 0.0);
		glTexCoord2f(0.0f, 1.0f); 
		glVertex3f (O.getE(), O.getN(), 0.3);
		glTexCoord2f(0.0f, 0.0f); 
		glVertex3f (O.getE(), O.getS(), 0.3);
		glTexCoord2f(1.0f, 0.0f); 
	    glVertex3f (O.getE(), O.getS(), 0.0);

	  }
		else if(O.dir=='n'){
		glNormal3f( 0.0f, 0.0f, 1.0f);					// Normal Pointing Towards Viewer
		glTexCoord2f(1.0f, 1.0f); 
		glVertex3f (O.getE(), O.getN(),0.3);
		glTexCoord2f(0.0f, 1.0f); 
		glVertex3f (O.getW(), O.getN(), 0.3);
		glTexCoord2f(0.0f, 0.0f); 
		glVertex3f (O.getW(), O.getS(), 0.3);
		glTexCoord2f(1.0f, 0.0f); 
	    glVertex3f (O.getE(), O.getS(), 0.3);

		//Outward Facing Quad
		glNormal3f( 0.0f, 1.0f, 0.0f);					// Normal Pointing Up
		glTexCoord2f(1.0f, 1.0f); 
		glVertex3f (O.getE(), O.getN(), 0.0);
		glTexCoord2f(0.0f, 1.0f); 
		glVertex3f (O.getW(), O.getN(), 0.0);
		glTexCoord2f(0.0f, 0.0f); 
		glVertex3f (O.getW(), O.getN(), 0.3);
		glTexCoord2f(1.0f, 0.0f); 
	    glVertex3f (O.getE(), O.getN(), 0.3);
		
		//Inward Facing Quad
		glNormal3f( 0.0f,-1.0f, 0.0f);					// Normal Pointing Down
		glTexCoord2f(1.0f, 1.0f); 
		glVertex3f (O.getE(), O.getS(), 0.3);
		glTexCoord2f(0.0f, 1.0f); 
		glVertex3f (O.getW(), O.getS(), 0.3);
		glTexCoord2f(0.0f, 0.0f); 
		glVertex3f (O.getW(), O.getS(), 0.0);
		glTexCoord2f(1.0f, 0.0f); 
	    glVertex3f (O.getE(), O.getS(), 0.0);

		//Left Facing Quad
		glNormal3f( -1.0f, 0.0f, 0.0f);					// Normal Pointing Left
		glTexCoord2f(1.0f, 1.0f); 
		glVertex3f (O.getW(), O.getN(), 0.3);
		glTexCoord2f(0.0f, 1.0f); 
		glVertex3f (O.getW(), O.getN(), 0.0);
		glTexCoord2f(0.0f, 0.0f); 
		glVertex3f (O.getW(),O.getS(), 0.0);
		glTexCoord2f(1.0f, 0.0f); 
	    glVertex3f (O.getW(), O.getS(), 0.3);
		
		//Right Facing Quad
		glNormal3f(1.0f, 0.0f, 0.0f);					// Normal Pointing Right
		glTexCoord2f(1.0f, 1.0f); 
		glVertex3f (O.getE(), O.getN(), 0.0);
		glTexCoord2f(0.0f, 1.0f); 
		glVertex3f (O.getE(), O.getN(), 0.3);
		glTexCoord2f(0.0f, 0.0f); 
		glVertex3f (O.getE(), O.getS(), 0.3);
		glTexCoord2f(1.0f, 0.0f); 
	    glVertex3f (O.getE(), O.getS(), 0.0);

		}
		
	}
		glEnd();
		glDisable(GL_TEXTURE_2D);

}
void drawField() {

		
	//Do texture mapping
       glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, LandTexture);

	glBegin(GL_QUADS);
		glNormal3f( 0.0f, 0.0f, 1.0f);					// Normal Pointing Towards Viewer
		glTexCoord2f(1.0f, 1.0f); 
	    glVertex3f (1.0, 1.0, 0.0);
		glTexCoord2f(0.0f, 1.0f); 
	    glVertex3f (-1.0, 1.0, 0.0);
		glTexCoord2f(0.0f, 0.0f); 
		glVertex3f (-1.0, -1.0, 0.0);
		glTexCoord2f(1.0f, 0.0f); 
	    glVertex3f (1.0, -1.0, 0.0);
   glEnd();

  glDisable(GL_TEXTURE_2D);
   
   
}

void drawTanks() {

	float x, y;
	//Tank 1
	tank dTank;
	//Tank2
	tank uTank;

	dTank = curGame.getTank1();
	if (dTank.isHit == false) {
		x = dTank.location.x/50;
		y = dTank.location.y/50;
		if (x > 1) x = 1;
		if (x < -1) x = -1;
		if (y > 1) y = 1;
		if (y < -1) y = -1;

			glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, TankTexture);

		if (playing == true) glTranslatef(x,y,0.0);
		else {
			glTranslatef(0,0,.3);
		}
		glRotatef(dTank.angle,0.0f,0.0f,1.0f);
		glBegin(GL_QUADS);
		
			glNormal3f( 0.0f, 0.0f, 1.0f);					// Normal Pointing Towards Viewer
			glColor3f(0.0f,0.0f,0.0f);	//Brown
			glTexCoord2f(1.0f, 1.0f); 
			glVertex3f (0.05,0.05, 0.02);
			glTexCoord2f(0.0f, 1.0f); 
			glVertex3f (-0.05, 0.05, 0.02);
			glTexCoord2f(0.0f, 0.0f); 
			glVertex3f (-0.05, -0.05, 0.02);
			glTexCoord2f(1.0f, 0.0f); 
			glVertex3f (0.05, -0.05, 0.02);

			glNormal3f( -1.0f, 0.0f, 0.0f);					// Normal Pointing Towards Viewer
			glColor3f(0.0f,0.0f,0.0f);	//Brown
			glTexCoord2f(1.0f, 1.0f); 
			glVertex3f (-0.05, 0.05, 0.02);
			glTexCoord2f(0.0f, 1.0f); 
			glVertex3f (-0.05, 0.05, 0.0);
			glTexCoord2f(0.0f, 0.0f); 
			glVertex3f (-0.05, -0.05, 0.0);
			glTexCoord2f(1.0f, 0.0f); 
			glVertex3f (-0.05, -0.05, 0.02);
			
			glNormal3f( 1.0f, 0.0f, 0.0f);					// Normal Pointing Towards Viewer
			glColor3f(0.0f,0.0f,0.0f);	//Brown
			glTexCoord2f(1.0f, 1.0f); 
			glVertex3f (0.05,0.05, 0.02);
			glTexCoord2f(0.0f, 1.0f); 
			glVertex3f (0.05,0.05, 0.0);
			glTexCoord2f(0.0f, 0.0f); 
			glVertex3f (0.05,-0.05, 0.0);
			glTexCoord2f(1.0f, 0.0f); 
			glVertex3f (0.05,-0.05, 0.02);
			
			glNormal3f( 0.0f, -1.0f, 0.0f);					// Normal Pointing Towards Viewer
			glColor3f(0.0f,0.0f,0.0f);	//Brown
			glTexCoord2f(1.0f, 1.0f); 
			glVertex3f (0.05,-0.05, 0.02);
			glTexCoord2f(0.0f, 1.0f); 
			glVertex3f (-0.05, -0.05, 0.02);
			glTexCoord2f(0.0f, 0.0f); 
			glVertex3f (-0.05, -0.05, 0.0);
			glTexCoord2f(1.0f, 0.0f); 
			glVertex3f (0.05,-0.05, 0.0);

			glNormal3f( 0.0f, 1.0f, 0.0f);					// Normal Pointing Towards Viewer
			glColor3f(0.0f,0.0f,0.0f);	//Brown
			glTexCoord2f(1.0f, 1.0f); 
			glVertex3f (0.05,0.05, 0.0);
			glTexCoord2f(0.0f, 1.0f); 
			glVertex3f (-0.05, 0.05, 0.0);
			glTexCoord2f(0.0f, 0.0f); 
			glVertex3f (-0.05, 0.05, 0.02);
			glTexCoord2f(1.0f, 0.0f); 
			glVertex3f (0.05,0.05, 0.02);

		glEnd();
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_LIGHT1);
		glDisable(GL_LIGHTING);
		

		glBegin(GL_QUADS);
			glColor3f(0.5f,0.0f,0.0f);	//Black
			glVertex3f (0.005,0.08, 0.023);
			glVertex3f (-0.005, 0.08, 0.023);
			glVertex3f (-0.005, 0.0, 0.023);
			glVertex3f (0.005, 0.0, 0.023);
	   glEnd();

		glEnable(GL_LIGHT1);										// Enable Light One
		glEnable(GL_LIGHTING);										// Enable Lighting
		
		glRotatef(-dTank.angle,0.0f,0.0f,1.0f);
   		if (playing == true) glTranslatef(-x,-y,0.0);
		else {
			glTranslatef(0,0,-.3);
		}
	}

	uTank = curGame.getTank2();
	if (uTank.isHit == false) {
		x = uTank.location.x/50;
		y = uTank.location.y/50;
		if (x > 1) x = 1;
		if (x < -1) x = -1;
		if (y > 1) y = 1;
		if (y < -1) y = -1;

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, TankTexture);
		if (playing == true) glTranslatef(x,y,0.0);
		else {
			glTranslatef(0,0,.3);
		}
		glRotatef(uTank.angle,0.0f,0.0f,1.0f);
		glBegin(GL_QUADS);

glNormal3f( 0.0f, 0.0f, 1.0f);					// Normal Pointing Towards Viewer
			glColor3f(0.0f,0.0f,0.0f);	//Brown
			glTexCoord2f(1.0f, 1.0f); 
			glVertex3f (0.05,0.05, 0.02);
			glTexCoord2f(0.0f, 1.0f); 
			glVertex3f (-0.05, 0.05, 0.02);
			glTexCoord2f(0.0f, 0.0f); 
			glVertex3f (-0.05, -0.05, 0.02);
			glTexCoord2f(1.0f, 0.0f); 
			glVertex3f (0.05, -0.05, 0.02);

			glNormal3f( -1.0f, 0.0f, 0.0f);					// Normal Pointing Towards Viewer
			glColor3f(0.0f,0.0f,0.0f);	//Brown
			glTexCoord2f(1.0f, 1.0f); 
			glVertex3f (-0.05, 0.05, 0.02);
			glTexCoord2f(0.0f, 1.0f); 
			glVertex3f (-0.05, 0.05, 0.0);
			glTexCoord2f(0.0f, 0.0f); 
			glVertex3f (-0.05, -0.05, 0.0);
			glTexCoord2f(1.0f, 0.0f); 
			glVertex3f (-0.05, -0.05, 0.02);
			
			glNormal3f( 1.0f, 0.0f, 0.0f);					// Normal Pointing Towards Viewer
			glColor3f(0.0f,0.0f,0.0f);	//Brown
			glTexCoord2f(1.0f, 1.0f); 
			glVertex3f (0.05,0.05, 0.02);
			glTexCoord2f(0.0f, 1.0f); 
			glVertex3f (0.05,0.05, 0.0);
			glTexCoord2f(0.0f, 0.0f); 
			glVertex3f (0.05,-0.05, 0.0);
			glTexCoord2f(1.0f, 0.0f); 
			glVertex3f (0.05,-0.05, 0.02);
			
			glNormal3f( 0.0f, -1.0f, 0.0f);					// Normal Pointing Towards Viewer
			glColor3f(0.0f,0.0f,0.0f);	//Brown
			glTexCoord2f(1.0f, 1.0f); 
			glVertex3f (0.05,-0.05, 0.02);
			glTexCoord2f(0.0f, 1.0f); 
			glVertex3f (-0.05, -0.05, 0.02);
			glTexCoord2f(0.0f, 0.0f); 
			glVertex3f (-0.05, -0.05, 0.0);
			glTexCoord2f(1.0f, 0.0f); 
			glVertex3f (0.05,-0.05, 0.0);

			glNormal3f( 0.0f, 1.0f, 0.0f);					// Normal Pointing Towards Viewer
			glColor3f(0.0f,0.0f,0.0f);	//Brown
			glTexCoord2f(1.0f, 1.0f); 
			glVertex3f (0.05,0.05, 0.0);
			glTexCoord2f(0.0f, 1.0f); 
			glVertex3f (-0.05, 0.05, 0.0);
			glTexCoord2f(0.0f, 0.0f); 
			glVertex3f (-0.05, 0.05, 0.02);
			glTexCoord2f(1.0f, 0.0f); 
			glVertex3f (0.05,0.05, 0.02);


	   glEnd();
	   glDisable(GL_TEXTURE_2D);
	   glDisable(GL_LIGHT1);
		glDisable(GL_LIGHTING);

	   glBegin(GL_QUADS);
			glColor3f(0.0f,0.0f,5.0f);	//Black
			glVertex3f (0.005,0.08, 0.023);
			glVertex3f (-0.005, 0.08, 0.023);
			glVertex3f (-0.005, 0.0, 0.023);
			glVertex3f (0.005, 0.0, 0.023);
	   glEnd();
		glEnable(GL_LIGHT1);										// Enable Light One
		glEnable(GL_LIGHTING);										// Enable Lighting
	}
}

void drawBullets() {
	
	float x, y;

   vector<bullet>::iterator iBullet;
   for (iBullet = curGame.vBullets.begin(); iBullet != curGame.vBullets.end(); iBullet++) {
	   glPushMatrix();
			x = iBullet->location.x/50;
			y = iBullet->location.y/50;
			glColor3f(1.0f,1.0f,0.0f);	//Black
			glTranslatef(x,y,0.0);
			glRotatef(iBullet->angle,0.0f,0.0f,1.0f);
			glBegin(GL_QUADS);
				glColor3f(1.0f,1.0f,1.0f);	//Black
				glNormal3f( 0.0f, 0.0f, 1.0f);					// Normal Pointing Towards Viewer
				glVertex3f (0.005, 0.01, 0.023);
				glVertex3f (-0.005, 0.01, 0.023);
				glVertex3f (-0.005, 0.0, 0.023);
				glVertex3f (0.005, 0.0, 0.023);
			glEnd();
		glPopMatrix();// Reset The Current Modelview Matrix
	}
}

int DrawGLScene(GLvoid)									// Here's Where We Do All The Drawing
{
	curGame.updateBullets();
	if (curGame.Tank1.isHit) playing = false;
	if (curGame.Tank2.isHit) playing = false;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
	glLoadIdentity();
	glEnable(GL_LIGHT1);										// Enable Light One
	glEnable(GL_LIGHTING);										// Enable Lighting
	//TODO: 
		glTranslatef(0.0,0.0,-2.5);
		glRotatef(-40,1.0f,0.0f,0.0f);
		if (playing == false) {
			rotangle -= 1;
		}
		glRotatef(rotangle,0.0f,0.0f,1.0f);
		//Draw field
		glPushMatrix();
			drawField();
		glPopMatrix();// Reset The Current Modelview Matrix
		
		
		glPushMatrix();
			drawObstacles();
		glPopMatrix();// Reset The Current Modelview Matrix
		
		//Draw bullets
		glPushMatrix();
			drawBullets();
		glPopMatrix();// Reset The Current Modelview Matrix
		//Draw Tanks
		glPushMatrix();
			drawTanks();
		glPopMatrix();// Reset The Current Modelview Matrix
	
	if (playing == false) {
		glDisable(GL_LIGHT1);										// Enable Light One
		glDisable(GL_LIGHTING);										// Enable Lighting
		glLoadIdentity();
		glTranslatef(0.0f,0.0f,-1.0f);						// Move One Unit Into The Screen
		// Pulsing Colors Based On Text Position
		glColor3f(1.0f*float(cos(cnt1)),1.0f*float(sin(cnt2)),1.0f-0.5f*float(cos(cnt1+cnt2)));
		// Position The Text On The Screen
		glRasterPos2f(-0.45f+0.05f*float(cos(cnt1)), 0.32f*float(sin(cnt2)));
		if (curGame.Tank1.isHit == false) {
 			glPrint("Tank 1 is the winner!");	// Print GL Text To The Screen
		}
		else {
			glPrint("Tank 2 is the winner!");	// Print GL Text To The Screen
		}
		cnt1+=0.051f;										// Increase The First Counter
		cnt2+=0.005f;										// Increase The First Counter
	}
	//Draw explosions
	return TRUE;										// Everything Went OK
}

GLvoid KillGLWindow(GLvoid)								// Properly Kill The Window
{
	if (fullscreen)										// Are We In Fullscreen Mode?
	{
		ChangeDisplaySettings(NULL,0);					// If So Switch Back To The Desktop
		ShowCursor(TRUE);								// Show Mouse Pointer
	}

	if (hRC)											// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL,NULL))					// Are We Able To Release The DC And RC Contexts?
		{
			MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))						// Are We Able To Delete The RC?
		{
			MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
		hRC=NULL;										// Set RC To NULL
	}

	if (hDC && !ReleaseDC(hWnd,hDC))					// Are We Able To Release The DC
	{
		MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hDC=NULL;										// Set DC To NULL
	}

	if (hWnd && !DestroyWindow(hWnd))					// Are We Able To Destroy The Window?
	{
		MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;										// Set hWnd To NULL
	}

	if (!UnregisterClass("OpenGL",hInstance))			// Are We Able To Unregister Class
	{
		MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;									// Set hInstance To NULL
	}
	KillFont();						// Destroy The Font

}

/*	This Code Creates Our OpenGL Window.  Parameters Are:					*
 *	title			- Title To Appear At The Top Of The Window				*
 *	width			- Width Of The GL Window Or Fullscreen Mode				*
 *	height			- Height Of The GL Window Or Fullscreen Mode			*
 *	bits			- Number Of Bits To Use For Color (8/16/24/32)			*
 *	fullscreenflag	- Use Fullscreen Mode (TRUE) Or Windowed Mode (FALSE)	*/
 
BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag)
{
	GLuint		PixelFormat;			// Holds The Results After Searching For A Match
	WNDCLASS	wc;						// Windows Class Structure
	DWORD		dwExStyle;				// Window Extended Style
	DWORD		dwStyle;				// Window Style
	RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left=(long)0;			// Set Left Value To 0
	WindowRect.right=(long)width;		// Set Right Value To Requested Width
	WindowRect.top=(long)0;				// Set Top Value To 0
	WindowRect.bottom=(long)height;		// Set Bottom Value To Requested Height

	fullscreen=fullscreenflag;			// Set The Global Fullscreen Flag

	hInstance			= GetModuleHandle(NULL);				// Grab An Instance For Our Window
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc		= (WNDPROC) WndProc;					// WndProc Handles Messages
	wc.cbClsExtra		= 0;									// No Extra Window Data
	wc.cbWndExtra		= 0;									// No Extra Window Data
	wc.hInstance		= hInstance;							// Set The Instance
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground	= NULL;									// No Background Required For GL
	wc.lpszMenuName		= NULL;									// We Don't Want A Menu
	wc.lpszClassName	= "OpenGL";								// Set The Class Name

	if (!RegisterClass(&wc))									// Attempt To Register The Window Class
	{
		MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;											// Return FALSE
	}
	
	if (fullscreen)												// Attempt Fullscreen Mode?
	{
		DEVMODE dmScreenSettings;								// Device Mode
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	// Makes Sure Memory's Cleared
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);		// Size Of The Devmode Structure
		dmScreenSettings.dmPelsWidth	= width;				// Selected Screen Width
		dmScreenSettings.dmPelsHeight	= height;				// Selected Screen Height
		dmScreenSettings.dmBitsPerPel	= bits;					// Selected Bits Per Pixel
		dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

		// Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
		if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
		{
			// If The Mode Fails, Offer Two Options.  Quit Or Use Windowed Mode.
			if (MessageBox(NULL,"The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?","NeHe GL",MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
			{
				fullscreen=FALSE;		// Windowed Mode Selected.  Fullscreen = FALSE
			}
			else
			{
				// Pop Up A Message Box Letting User Know The Program Is Closing.
				MessageBox(NULL,"Program Will Now Close.","ERROR",MB_OK|MB_ICONSTOP);
				return FALSE;									// Return FALSE
			}
		}
	}

	if (fullscreen)												// Are We Still In Fullscreen Mode?
	{
		dwExStyle=WS_EX_APPWINDOW;								// Window Extended Style
		dwStyle=WS_POPUP;										// Windows Style
		ShowCursor(FALSE);										// Hide Mouse Pointer
	}
	else
	{
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
		dwStyle=WS_OVERLAPPEDWINDOW;							// Windows Style
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

	// Create The Window
	if (!(hWnd=CreateWindowEx(	dwExStyle,							// Extended Style For The Window
								"OpenGL",							// Class Name
								title,								// Window Title
								dwStyle |							// Defined Window Style
								WS_CLIPSIBLINGS |					// Required Window Style
								WS_CLIPCHILDREN,					// Required Window Style
								0, 0,								// Window Position
								WindowRect.right-WindowRect.left,	// Calculate Window Width
								WindowRect.bottom-WindowRect.top,	// Calculate Window Height
								NULL,								// No Parent Window
								NULL,								// No Menu
								hInstance,							// Instance
								NULL)))								// Dont Pass Anything To WM_CREATE
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	static	PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		bits,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// 16Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};
	
	if (!(hDC=GetDC(hWnd)))							// Did We Get A Device Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))	// Did Windows Find A Matching Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if(!SetPixelFormat(hDC,PixelFormat,&pfd))		// Are We Able To Set The Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(hRC=wglCreateContext(hDC)))				// Are We Able To Get A Rendering Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if(!wglMakeCurrent(hDC,hRC))					// Try To Activate The Rendering Context
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	ShowWindow(hWnd,SW_SHOW);						// Show The Window
	SetForegroundWindow(hWnd);						// Slightly Higher Priority
	SetFocus(hWnd);									// Sets Keyboard Focus To The Window
	ReSizeGLScene(width, height);					// Set Up Our Perspective GL Screen

	if (!InitGL())									// Initialize Our Newly Created GL Window
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Initialization Failed.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	return TRUE;									// Success
}

LRESULT CALLBACK WndProc(	HWND	hWnd,			// Handle For This Window
							UINT	uMsg,			// Message For This Window
							WPARAM	wParam,			// Additional Message Information
							LPARAM	lParam)			// Additional Message Information
{
	switch (uMsg)									// Check For Windows Messages
	{
		case WM_ACTIVATE:							// Watch For Window Activate Message
		{
			if (!HIWORD(wParam))					// Check Minimization State
			{
				active=TRUE;						// Program Is Active
			}
			else
			{
				active=FALSE;						// Program Is No Longer Active
			}

			return 0;								// Return To The Message Loop
		}

		case WM_SYSCOMMAND:							// Intercept System Commands
		{
			switch (wParam)							// Check System Calls
			{
				case SC_SCREENSAVE:					// Screensaver Trying To Start?
				case SC_MONITORPOWER:				// Monitor Trying To Enter Powersave?
				return 0;							// Prevent From Happening
			}
			break;									// Exit
		}

		case WM_CLOSE:								// Did We Receive A Close Message?
		{
			PostQuitMessage(0);						// Send A Quit Message
			return 0;								// Jump Back
		}

		case WM_KEYDOWN:							// Is A Key Being Held Down?
		{
			keys[wParam] = TRUE;					// If So, Mark It As TRUE
			return 0;								// Jump Back
		}

		case WM_KEYUP:								// Has A Key Been Released?
		{
			keys[wParam] = FALSE;					// If So, Mark It As FALSE
			return 0;								// Jump Back
		}

		case WM_SIZE:								// Resize The OpenGL Window
		{
			ReSizeGLScene(LOWORD(lParam),HIWORD(lParam));  // LoWord=Width, HiWord=Height
			return 0;								// Jump Back
		}
	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

int WINAPI WinMain(	HINSTANCE	hInstance,			// Instance
					HINSTANCE	hPrevInstance,		// Previous Instance
					LPSTR		lpCmdLine,			// Command Line Parameters
					int			nCmdShow)			// Window Show State
{
	MSG		msg;									// Windows Message Structure
	BOOL	done=FALSE;								// Bool Variable To Exit Loop

	//readInFile(lpCmdLine);
	// Ask The User Which Screen Mode They Prefer
	//if (MessageBox(NULL,"Would You Like To Run In Fullscreen Mode?", "Start FullScreen?",MB_YESNO|MB_ICONQUESTION)==IDNO)
	//{
	//	fullscreen=FALSE;							// Windowed Mode
	//}
	fullscreen = false;

	// Create Our OpenGL Window
	if (!CreateGLWindow("Tank Attack!",SCREENWIDTH,SCREENHEIGHT,16,fullscreen))
	{
		return 0;									// Quit If Window Was Not Created
	}
	
	//TODO:	Spawn thread to handle updating of the bullets


	while(!done)									// Loop That Runs While done=FALSE
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))	// Is There A Message Waiting?
		{
			if (msg.message==WM_QUIT)				// Have We Received A Quit Message?
			{
				done=TRUE;							// If So done=TRUE
			}
			else									// If Not, Deal With Window Messages
			{
				TranslateMessage(&msg);				// Translate The Message
				DispatchMessage(&msg);				// Dispatch The Message
			}
		}
		else										// If There Are No Messages
		{
			// Draw The Scene.  Watch For ESC Key And Quit Messages From DrawGLScene()
			if (active)								// Program Active?
			{
				if (keys[VK_ESCAPE])				// Was ESC Pressed?
				{
					done=TRUE;						// ESC Signalled A Quit
				}
				else								// Not Time To Quit, Update Screen
				{
					DrawGLScene();					// Draw The Scene
					SwapBuffers(hDC);				// Swap Buffers (Double Buffering)
				}
			}

			if (keys[VK_F1])						// Is F1 Being Pressed?
			{
				keys[VK_F1]=FALSE;					// If So Make Key FALSE
				KillGLWindow();						// Kill Our Current Window
				fullscreen=!fullscreen;				// Toggle Fullscreen / Windowed Mode
				// Recreate Our OpenGL Window
				if (!CreateGLWindow("Tank Attack!",SCREENWIDTH,SCREENHEIGHT,16,fullscreen))
				{
					return 0;						// Quit If Window Was Not Created
				}
			}

			//keys for TANK 1
			if (keys[VK_UP])				// Is Up Arrow Being Pressed?
				{
					curGame.moveTank1('U');	
					//keys[VK_UP] = false;
				}
			if (keys[VK_DOWN])				// Is Down Arrow Being Pressed?
				{
					curGame.moveTank1('D');
					//keys[VK_DOWN] = false;
				}
			if (keys[VK_RIGHT])				// Is Right Arrow Being Pressed?
				{
					curGame.rotateTank1('R');
					keys[VK_RIGHT] = false;
				}
			if (keys[VK_LEFT])				// Is Left Arrow Being Pressed?
				{
					curGame.rotateTank1('L');
					keys[VK_LEFT] = false;
				}
			if (keys[VK_CONTROL]) 
			{
				curGame.fireBullet(1);
				//keys[VK_SPACE] = false;
			}

			//keys for TANK 2
			if (keys['W'])				// Is Up Arrow Being Pressed?
				{
					curGame.moveTank2('U');	
					//keys[VK_UP] = false;
				}
			if (keys['Z'])				// Is Down Arrow Being Pressed?
				{
					curGame.moveTank2('D');
					//keys[VK_DOWN] = false;
				}
			if (keys['D'])				// Is Right Arrow Being Pressed?
				{
					curGame.rotateTank2('R');
					keys['D'] = false;
				}
			if (keys['A'])				// Is Left Arrow Being Pressed?
				{
					curGame.rotateTank2('L');
					keys['A'] = false;
				}
			if (keys[VK_SPACE]) 
			{
				if (playing == true) curGame.fireBullet(2);
				else {
					ResetGame();
					keys[VK_SPACE] = false;
				}
				
			}
			if (keys['R']) 
			{
				rotangle -=1;
				keys['R'] = false;
			}
		}
	}

	// Shutdown
	KillGLWindow();									// Kill The Window
	return (msg.wParam);							// Exit The Program
}

