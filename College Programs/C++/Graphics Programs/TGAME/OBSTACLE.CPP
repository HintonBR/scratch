#include "obstacle.h"


Obstacle::Obstacle(){}

Obstacle::Obstacle(float xP, float yP, float w, float h)
{
   x = xP;
   y = yP;
   width = w;
   height = h;
   //compute wall direction
   if(w>h)
   {
	  if(y>0)
	  {
	   dir = 'n';
	  }
	  else
	  {
	   dir = 's';
	  }

   }
   else
   {
	 if (x>0)
		 dir = 'r';
	 else	dir = 'l';	
   }
}

Obstacle::~Obstacle(){}

float Obstacle::getE()
{
	return (x+(width/2));
}

float Obstacle::getW()
{
	return (x-(width/2));
}

float Obstacle::getN()
{
	return (y+(height/2));
}

float Obstacle::getS()
{
	return (y-(height/2));
}

bool Obstacle::hitTest(int x, int y, float wd, float h)
{
/*	
	int topLeftX = getW()*50; 
	int topLeftY = getN()*50; 
	int bottomRightX = getE()*50; 
	int bottomRightY = getS()*50; 
	//second rectangle
	int topLX = x - (wd/2); 
	int topLY = y + (h/2); 
	int bottomRX = x + (wd/2); 
	int bottomRY = y - (h/2); 

	// Determine if current object intersects with Rectangle rect 
    if (bottomRightX < topLX ) 
        // Rectangles don't intersect on X axis 
        return false; 
    if (bottomRX < topLeftX) 
        // Rectangles don't intersect on X axis 
        return false; 
    if (bottomRightY < topLY) 
        // Rectangles don't intersect on Y axis 
        return false; 
    if (bottomRY < topLeftY) 
        // Rectangles don't intersect on Y axis 
        return false; 
    // Rectangles must intersect 
	return true;
	/*/
	//my rectangle
	int x1min = getW()*50;
	int x1max = getE()*50; 
	int y1max = getN()*50; 
	int y1min = getS()*50; 
	//second rectangle
	int x2min  = x - (wd/2); 
	int x2max= x + (wd/2); 
	int y2max= y + (h/2); 
	int y2min= y - (h/2); 

	bool n=false,e=false,w=false,s=false;

	n = (y1min<y2max);
	s = (y1max>y2min);
	e = (x1min<x2max);
	w = (x1max>x2min);

	//if((n&&(e||w))||(s&&(e||w)))
	if(!n||!e||!w||!s)
	{return false;}	// Determine if current object intersects with Rectangle rect 
    return true;
}
