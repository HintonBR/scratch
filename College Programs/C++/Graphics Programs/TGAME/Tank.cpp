// tank.cpp: implementation of the tank class.
//
//////////////////////////////////////////////////////////////////////

#include "tank.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

tank::tank()
{
	location.x = 0;
	location.y = 0;
	width = 3;
	height = 3;
	angle = 0;
	fireSpeed = 300;
	moveSpeed = 100;
	_ftime(&lastFireTime);
	_ftime(&lastMoveTime);
	isHit = false;
}

tank::tank(float x, float y, float w, float h)
{
	location.x = x;
	location.y = y;
	width = w;
	height = h;
	angle = 0;
	fireSpeed = 300;
	moveSpeed = 100;
	_ftime(&lastFireTime);
	_ftime(&lastMoveTime);
	isHit = false;
}

tank::~tank()
{

}

bullet tank::fire(bool* fired) {
	bullet newBullet = bullet();
	*fired = false;
	if(isValidTime(fireSpeed, lastFireTime))
	{
		newBullet.angle = angle;
		newBullet.location.x = location.x;
		newBullet.location.y = location.y;
		_ftime(&lastFireTime);
		*fired = true;
		if (angle == 0){
			newBullet.location.y += 5;
		}
		else if (angle == 45){
			newBullet.location.x -= 4;
			newBullet.location.y += 4;
		}
		else if (angle == 90){
			newBullet.location.x -= 5;
		}
		else if (angle == 135){
			newBullet.location.x -= 4;
			newBullet.location.y -= 4;
		}
		else if (angle == 180) {
			newBullet.location.y -= 5;
		}
		else if (angle == 225) {
			newBullet.location.x += 4;
			newBullet.location.y -= 4;
		}
		else if (angle == 270){
			newBullet.location.x += 5;
		}
		else if (angle == 315) {
			newBullet.location.x += 4;
			newBullet.location.y += 4;
		}
	}
		return newBullet;
}
/*
bool tank::hitTest(int x, int y, float w, float h)
{
	//my rectangle
	int topLeftX = (int)location.x - (width/2); 
	int topLeftY = (int)location.y + (height/2); 
	int bottomRightX = (int)location.x + (width/2); 
	int bottomRightY = (int)location.y - (height/2); 
	//second rectangle
	int topLX = x - (w/2); 
	int topLY = y + (h/2); 
	int bottomRX = x + (w/2); 
	int bottomRY = y - (h/2); 

	// Determine if current object intersects with Rectangle rect 
    if (bottomRightX > topLX ) 
        // Rectangles don't intersect on X axis 
        return false; 
    if (bottomRX > topLeftX) 
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
}
*/
bool tank::hitTest(float x, float y, float wd, float h)
{
	//my rectangle
	float x1min = location.x - (width/2); 
	float x1max = location.x + (width/2); 
	float y1max = location.y + (height/2); 
	float y1min = location.y - (height/2); 
	//second rectangle
	float x2min  = x - (wd/2); 
	float x2max= x + (wd/2); 
	float y2max= y + (h/2); 
	float y2min= y - (h/2); 

	bool n=false,e=false,w=false,s=false;

	n = (y1min<y2max);
	s = (y1max>y2min);
	e = (x1min<x2max);
	w = (x1max>x2min);

	//if((n&&(e||w))||(s&&(e||w)))
	if(!n||!e||!w||!s)
	{return false;}
	// Determine if current object intersects with Rectangle rect 
    return true;
}

bool tank::move(char direction)
{

	
	bool isvalid = false;
	if(isValidTime(moveSpeed, lastMoveTime))
	{
		
		cout << "tank Moved.\n" << endl;
		isvalid = true;
		_ftime(&lastMoveTime);
		if (direction == 'U'){ // Moving tank UP
			if (angle == 0){
				location.y++;
			}
			else if (angle == 45){
				location.x--;
				location.y++;
			}
			else if (angle == 90){
				location.x--;
			}
			else if (angle == 135){
				location.x--;
				location.y--;
			}
			else if (angle == 180) {
				location.y--;
			}
			else if (angle == 225) {
				location.x++;
				location.y--;
			}
			else if (angle == 270){
				location.x++;
			}
			else if (angle == 315) {
				location.x++;
				location.y++;
			}
		}
		else if (direction == 'D') // Moving tank DOWN
		{
			if (angle == 0){
				location.y--;
			}
			else if (angle == 45){
				location.x++;
				location.y--;
			}
			else if (angle == 90){
				location.x++;
			}
			else if (angle == 135){
				location.x++;
				location.y++;
			}
			else if (angle == 180) {
				location.y++;
			}
			else if (angle == 225) {
				location.x--;
				location.y++;
			}
			else if (angle == 270){
				location.x--;
			}
			else if (angle == 315) {
				location.x--;
				location.y--;
			}
		}
	}
	else
		cout << "tank did not Move.\n" << endl;
	return isvalid;
}

bool tank::isValidTime(int speed, struct _timeb lTime)
{
	bool isvalid = true;
	struct _timeb currTime;
    
    _ftime( &currTime );
    printf( "Curr Time:\t\t\t%u\n", currTime.time );
    printf( "Curr milliseconds:\t\t\t%u\n", currTime.millitm );
	
    printf( "Last Time:\t\t\t%u\n", lTime.time );
    printf( "Last milliseconds:\t\t\t%u\n", lTime.millitm );

	if(currTime.time - lTime.time == 0){
		if(currTime.millitm - lTime.millitm < speed)
			isvalid = false;
	}
	else
	if(currTime.time - lTime.time == 1){
		if ( (1000 - lTime.millitm) + currTime.millitm < speed)
			isvalid = false;
	}
	return isvalid;
}

void tank::rotate(char direction)
{
	if (direction == 'L'){
		angle = angle + 45;
		if (angle == 360)
			angle = 0;
	}
	else
	if (direction == 'R'){
		if (angle==0) angle = 360;
		angle = angle - 45;
	}
}

point tank::getMovePoint(char direction)
{
  point p;
  int x = location.x;
  int y = location.y;
   
	if (direction == 'U'){ // Moving tank UP
			if (angle == 0){
				y++;
			}
			else if (angle == 45){
				x--;
				y++;
			}
			else if (angle == 90){
				x--;
			}
			else if (angle == 135){
				x--;
				y--;
			}
			else if (angle == 180) {
				y--;
			}
			else if (angle == 225) {
				x++;
				y--;
			}
			else if (angle == 270){
				x++;
			}
			else if (angle == 315) {
				x++;
				y++;
			}
		}
		else if (direction == 'D') // Moving tank DOWN
		{
			if (angle == 0){
				y--;
			}
			else if (angle == 45){
				x++;
				y--;
			}
			else if (angle == 90){
				x++;
			}
			else if (angle == 135){
				x++;
				y++;
			}
			else if (angle == 180) {
				y++;
			}
			else if (angle == 225) {
				x--;
				y++;
			}
			else if (angle == 270){
				x--;
			}
			else if (angle == 315) {
				x--;
				y--;
			}
		}
	
		p.x = x;
		p.y = y;

		return p;
}
