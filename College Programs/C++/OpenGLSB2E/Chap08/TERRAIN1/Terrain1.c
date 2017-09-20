/*
 * Terrain example 1 from Chapter 8.
 *
 * Written by Michael Sweet
 */

#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#ifndef WIN32
#  include <sys/time.h>
#endif /* !WIN32 */
#ifndef M_PI
#  define M_PI	(double)3.14159265358979323846
#endif /* !M_PI */
#include "texture.h"


/*
 * Constants:
 */

#define TERRAIN_SIZE    4000.0 /* Size of terrain database */
#define TERRAIN_VIEW    1000.0 /* Viewable distance in terrain database */


/*
 * Globals...
 */

int        Width;             /* Width of window */
int        Height;            /* Height of window */
double     LastTime;          /* Last update time */
int	   MouseStartX;       /* Initial mouse X position */
int        MouseStartY;       /* Initial mouse Y position */
int	   MouseX;            /* Mouse X position */
int        MouseY;            /* Mouse Y position */
int        ViewAngle = 0;     /* Viewing angle */
GLenum     PolyMode = GL_FILL;/* Polygon drawing mode */
int        UseTexturing = 1;  /* Use texturing? */
int        ShowSky = 1;       /* Show sky? */
GLfloat    Velocity = 10.0;   /* Flying speed */
GLfloat	   Position[3] =      /* Position of viewer */
    {
    0.0, 10.0, 0.0
    };
GLfloat	   Orientation[3] =   /* Orientation of viewer */
    {
    0.0, 0.0, 0.0
    };
GLuint     LandTexture;       /* Land texture object */
GLuint     SkyTexture;        /* Sky texture object */


/*
 * Functions...
 */

double GetClock(void);
void   Idle(void);
void   Joystick(unsigned state, int x, int y, int z);
void   Keyboard(unsigned char key, int x, int y);
void   Motion(int x, int y);
void   Mouse(int button, int state, int x, int y);
void   Redraw(void);
void   Resize(int width, int height);
void   Special(int key, int x, int y);


/*
 * 'main()' - Open a window and display a sphere and cube.
 */

int                /* O - Exit status */
main(int  argc,    /* I - Number of command-line arguments */
     char *argv[]) /* I - Command-line arguments */
    {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(792, 573);
    glutCreateWindow("Flat Terrain");

    glutDisplayFunc(Redraw);
    if (glutDeviceGet(GLUT_HAS_JOYSTICK))
        glutJoystickFunc(Joystick, 200);
    glutKeyboardFunc(Keyboard);
    glutMotionFunc(Motion);
    glutMouseFunc(Mouse);
    glutReshapeFunc(Resize);
    glutSpecialFunc(Special);

    LandTexture = TextureLoad("land.bmp", GL_FALSE, GL_LINEAR,
                              GL_LINEAR, GL_REPEAT);
    SkyTexture  = TextureLoad("sky.bmp", GL_FALSE, GL_LINEAR, GL_LINEAR,
                              GL_CLAMP);

    puts("QUICK HELP:");
    puts("");
    puts("ESC - Quit");
    puts("',' - Slow down, '<' - Slowest");
    puts("'.' - Speed up, '>' - Fastest");
    puts("'s' - Toggle sky/clouds");
    puts("'t' - Toggle texturing");
    puts("'W' - Toggle wireframe");

    glutMainLoop();
    return (0);
    }


/*
 * 'GetClock()' - Return an increasing clock time in seconds...
 */

double /* O - Time in seconds */
GetClock(void)
    {
#ifdef WIN32
    SYSTEMTIME t;      /* Current time of day */

    GetSystemTime(&t);
    return (((t.wHour * 60.0) + t.wMinute) * 60 + t.wSecond +
            t.wMilliseconds * 0.001);
#else /* UNIX */
    struct timeval t; /* Current time of day */

    gettimeofday(&t, NULL);
    return ((double)t.tv_sec + 0.000001 * (double)t.tv_usec);
#endif /* WIN32 */
    }


/*
 * 'Idle()' - Move the viewer and redraw...
 */

void
Idle(void)
    {
    GLfloat movex, movey; /* Scaled mouse movement */
    double  curtime;      /* Current time in milliseconds */
    GLfloat distance;     /* Distance to move */
    GLfloat cheading;     /* Cosine of heading */
    GLfloat sheading;     /* Sine of heading */
    GLfloat cpitch;       /* Cosine of pitch */
    GLfloat spitch;       /* Sine of pitch */
    static int    fps_count = 0;  /* Frames per second count */
    static double fps_time = 0.0; /* Frames per second time */

    /* Get the current system time to figure out how far to move. */
    curtime  = GetClock();
    distance = curtime - LastTime;
    LastTime = curtime;

    /*
     * See how far the mouse pointer is from the 'center' (click)
     * position.
     */

    movex = 0.01 * sqrt(Velocity) * (MouseX - MouseStartX);
    movey = 0.01 * sqrt(Velocity) * (MouseY - MouseStartY);

    /*
     * Adjust roll, pitch, and heading according to the current
     * mouse inputs and orientation.
     */

    Orientation[0] += distance * movey * cos(Orientation[2] * M_PI / 180.0);
    Orientation[1] += distance * movey * sin(Orientation[2] * M_PI / 180.0);
    Orientation[2] += distance * movex;

    if (Orientation[0] < -90.0)
        {
        Orientation[0] = -180.0 - Orientation[0];
	Orientation[2] += 180.0;
	}
    else if (Orientation[0] > 90.0)
        {
        Orientation[0] = 180.0 - Orientation[0];
	Orientation[2] -= 180.0;
	}

    if (Orientation[1] < 0.0)
        Orientation[1] += 360.0;
    else if (Orientation[1] > 360.0)
        Orientation[1] -= 360.0;

    if (Orientation[2] < -180.0)
        Orientation[2] += 360.0;
    else if (Orientation[2] > 180.0)
        Orientation[2] -= 360.0;

    /* Move based upon the current orientation... */
    cheading = cos(Orientation[1] * M_PI / 180.0);
    sheading = sin(Orientation[1] * M_PI / 180.0);
    cpitch   = cos(Orientation[0] * M_PI / 180.0);
    spitch   = sin(Orientation[0] * M_PI / 180.0);

    Position[0] += Velocity * distance * sheading * cpitch;
    Position[1] += Velocity * distance * spitch;
    Position[2] -= Velocity * distance * cheading * cpitch;

    /* Limit the viewer to the terrain... */
    if (Position[0] < (-0.45 * TERRAIN_SIZE))
        {
	Position[0]    = -0.45 * TERRAIN_SIZE;
	Orientation[1] += 10.0 * distance;
	}

    if (Position[0] > (0.45 * TERRAIN_SIZE))
        {
	Position[0]    = 0.45 * TERRAIN_SIZE;
	Orientation[1] += 10.0 * distance;
	}

    if (Position[2] < (-0.45 * TERRAIN_SIZE))
        {
	Position[2]    = -0.45 * TERRAIN_SIZE;
	Orientation[1] += 10.0 * distance;
	}

    if (Position[2] > (0.45 * TERRAIN_SIZE))
        {
	Position[2]    = 0.45 * TERRAIN_SIZE;
	Orientation[1] += 10.0 * distance;
	}

    if (Position[1] < 5.0)
        {
	Position[1]    = 5.0;
	Orientation[0] += 5.0 * distance;
	}

    if (Position[1] > (0.2 * TERRAIN_SIZE))
        {
	Position[1]    = 0.2 * TERRAIN_SIZE;
	Orientation[0] -= 5.0 * distance;
	}

    /* Update the frames-per-second value */
    fps_time += distance;
    fps_count ++;
    if (fps_count >= 20)
        {
	printf("\rFrames per second = %.2f    ", fps_count / fps_time);
	fflush(stdout);

	fps_time  = 0.0;
	fps_count = 0;
	}
    glutPostRedisplay();
    }


/*
 * 'Joystick()' - Handle joystick movement.
 */

void
Joystick(unsigned state,       /* I - Button state */
         int      x,           /* I - X position (-1000 to 1000) */
 	 int      y,           /* I - Y position (-1000 to 1000) */
	 int      z)           /* I - Z position (-1000 to 1000) */
    {
    static int last_state = 0; /* Last button state */

    if (last_state != state)
        {
	/* Button changed state; see what the new state is... */
	if (state && !last_state)
            {
    	    /* Start flying */
            MouseStartX = MouseX = x / 2;
	    MouseStartY = MouseY = y / 2;
	    LastTime    = GetClock();
            glutIdleFunc(Idle);
	    }
            else if (!state && last_state)
            {
	    /* Stop flying */
            glutIdleFunc((void (*)(void))0);
	    }

	last_state = state;
	}

    /* Update the joystick/mouse position */
    if (state)
        {
	MouseX = x / 2;
        MouseY = y / 2;
	}

    if (z > -999)
        Velocity = (999 - z) * 0.045 + 10.0;
    }


/*
 * 'Keyboard()' - Handle key presses...
 */

void
Keyboard(unsigned char key, /* I - Key that was pressed */
         int           x,   /* I - Mouse X position */
	 int           y)   /* I - Mouse Y position */
    {
    switch (key)
        {
	case 0x1b :
	    puts("");
	    exit(0);
	    break;
	case ',' :
	    if (Velocity > 5.0)
	        Velocity -= 5.0;
	    break;
	case '.' :
	    if (Velocity < 100.0)
	        Velocity += 5.0;
	    break;
	case 's' :
	    ShowSky = !ShowSky;
	    break;
	case 't' :
	    UseTexturing = !UseTexturing;
	    break;
	case 'W' :
	    if (PolyMode == GL_FILL)
	        PolyMode = GL_LINE;
	    else
	        PolyMode = GL_FILL;
	    break;
	}

    glutPostRedisplay();
    }


/*
 * 'Motion()' - Handle mouse pointer motion.
 */

void
Motion(int x, /* I - Current mouse X position */
       int y) /* I - Current mouse Y position */
    {
    MouseX = x;
    MouseY = y;
    }


/*
 * 'Mouse()' - Handle mouse button events.
 */

void
Mouse(int button, /* I - Button that changed */
      int state,  /* I - Current button states */
      int x,      /* I - Current mouse X position */
      int y)      /* I - Current mouse Y position */
    {
    if (state == GLUT_DOWN)
        {
	/* Start flying */
	MouseStartX = MouseX = x;
	MouseStartY = MouseY = y;
	LastTime    = GetClock();
        glutIdleFunc(Idle);
	}
    else
        {
	/* Stop flying */
        glutIdleFunc((void (*)(void))0);
	}
    }


/*
 * 'Redraw()' - Redraw the window...
 */

void
Redraw(void)
    {
    float theta, st, ct;

    /* Clear the window to light blue... */
    if (ShowSky && UseTexturing && PolyMode == GL_FILL)
        glClear(GL_DEPTH_BUFFER_BIT);
    else
        {
        glClearColor(0.75, 0.75, 1.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

    /* Setup viewing transformations for the current orientation... */
    glPushMatrix();
    glRotatef((float)ViewAngle, 0.0, 1.0, 0.0);
    glRotatef(Orientation[2], 0.0, 0.0, 1.0);
    glRotatef(Orientation[0], -1.0, 0.0, 0.0);
    glRotatef(Orientation[1], 0.0, 1.0, 0.0);
    glTranslatef(0.0, -Position[1], 0.0);

    glPolygonMode(GL_FRONT_AND_BACK, PolyMode);

    /* Draw the sky */
    if (UseTexturing && SkyTexture && ShowSky && PolyMode == GL_FILL)
        {
        glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, SkyTexture);

        glColor3f(1.0, 1.0, 1.0);
        glBegin(GL_TRIANGLE_FAN);
        glTexCoord2f(0.5, 0.5);
        glVertex3f(0.0, TERRAIN_VIEW, 0.0);
        for (theta = 0.0; theta < (2.1 * M_PI); theta += M_PI / 8)
            {
            ct = cos(theta);
            st = sin(theta);
            glTexCoord2f(0.5 + 0.3 * ct, 0.5 + 0.3 * st);
            glVertex3f(ct * TERRAIN_VIEW * 0.7071,
	               TERRAIN_VIEW * 0.7071,
	               st * TERRAIN_VIEW * 0.7071);
            }
        glEnd();

        glBegin(GL_TRIANGLE_STRIP);
        for (theta = 0.0; theta < (2.1 * M_PI); theta += M_PI / 8)
            {
            ct = cos(theta);
            st = sin(theta);
            glTexCoord2f(0.5 + 0.3 * ct, 0.5 + 0.3 * st);
            glVertex3f(ct * TERRAIN_VIEW * 0.7071,
	               TERRAIN_VIEW * 0.7071,
	               st * TERRAIN_VIEW * 0.7071);
            glTexCoord2f(0.5 + 0.5 * ct, 0.5 + 0.5 * st);
            glVertex3f(ct * TERRAIN_VIEW, -100.0,
	               st * TERRAIN_VIEW);
            }
        glEnd();
        }

    /* Then draw the terrain... */
    if (UseTexturing && LandTexture && PolyMode == GL_FILL)
        {
        glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, LandTexture);

        }

    glTranslatef(-Position[0], 0.0, -Position[2]);

    glColor3f(0.3, 0.8, 0.2);

    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-0.5 * TERRAIN_SIZE, 0.0, -0.5 * TERRAIN_SIZE);
    glTexCoord2f(50.0, 0.0);
    glVertex3f( 0.5 * TERRAIN_SIZE, 0.0, -0.5 * TERRAIN_SIZE);
    glTexCoord2f(50.0, 50.0);
    glVertex3f( 0.5 * TERRAIN_SIZE, 0.0,  0.5 * TERRAIN_SIZE);
    glTexCoord2f(0.0, 50.0);
    glVertex3f(-0.5 * TERRAIN_SIZE, 0.0,  0.5 * TERRAIN_SIZE);
    glEnd();

    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    /* Finish up */
    glutSwapBuffers();
    }


/*
 * 'Resize()' - Resize the window...
 */

void
Resize(int width,  /* I - Width of window */
       int height) /* I - Height of window */
    {
    /* Save the new width and height */
    Width  = width;
    Height = height;

    /* Reset the viewport... */
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(22.5, (float)width / (float)height, 4.0, TERRAIN_VIEW);
    glMatrixMode(GL_MODELVIEW);
    }


/*
 * 'Special()' - Handle special keys like left, right, up, and down.
 */

void
Special(int key, /* I - Key that was pressed */
        int x,   /* I - X location of the mouse pointer */
	int y)   /* I - Y location of the mouse pointer */
    {
    switch (key)
        {
	case GLUT_KEY_UP :
	    ViewAngle = 0;
	    glutPostRedisplay();
	    break;
	case GLUT_KEY_DOWN :
	    ViewAngle = 180;
	    glutPostRedisplay();
	    break;
	case GLUT_KEY_LEFT :
	    ViewAngle = (ViewAngle + 315) % 360;
	    glutPostRedisplay();
	    break;
	case GLUT_KEY_RIGHT :
	    ViewAngle = (ViewAngle + 45) % 360;
	    glutPostRedisplay();
	    break;
	}
    }
