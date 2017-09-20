// bullet.cpp: implementation of the bullet class.
//
//////////////////////////////////////////////////////////////////////

#include "bullet.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

bullet::bullet()
{
	moveSpeed = 50;
	_ftime(&lastMoveTime);
}

bullet::~bullet()
{

}

bool bullet::isValidTime(int speed, struct _timeb lTime)
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


void bullet::update() {
	if (isValidTime(moveSpeed, lastMoveTime)) {
		if (angle == 0){
			location.y+=2;
		}
		else if (angle == 45){
			location.x-=2;
			location.y+=2;
		}
		else if (angle == 90){
			location.x-=2;
		}
		else if (angle == 135){
			location.x-=2;
			location.y-=2;
		}
		else if (angle == 180) {
			location.y-=2;
		}
		else if (angle == 225) {
			location.x+=2;
			location.y-=2;
		}
		else if (angle == 270){
			location.x+=2;
		}
		else if (angle == 315) {
			location.x+=2;
			location.y+=2;
		}
		_ftime(&lastMoveTime);
		
		
	}
	
}