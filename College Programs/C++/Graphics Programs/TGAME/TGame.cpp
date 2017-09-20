// TGame.cpp: implementation of the TGame class.
//
//////////////////////////////////////////////////////////////////////

#include "TGame.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TGame::TGame()
{
	ResetTanks();
}

TGame::~TGame()
{

}

void TGame::ResetTanks() {
	Tank1.location.y = -40;
	Tank1.location.x = 0;
	Tank2.location.y = 40;
	Tank2.location.x = 40;
	Tank2.angle = 180;
	Tank1.isHit = false;
	Tank2.isHit = false;
}
void TGame::addObstacle(Obstacle O) {
  
  vObs.push_back(O);
}

void TGame::updateBullets() {
	char cObjectHit;
	hit hObj;
		
	vector<bullet>::iterator iBullet;
	vector<vector<bullet>::iterator> vRemoveList;
	vector<vector<bullet>::iterator>::iterator vRemoveListIt;
	for (iBullet = vBullets.begin(); iBullet != vBullets.end(); iBullet++) {
		iBullet->update();
		//Do Hit Test and if a Hit - insert a hit into the hit vector
		
 		cObjectHit = doHitTest(iBullet->location.x, iBullet->location.y);
		if (cObjectHit != 'n') {
			hObj = hit();
			hObj.cObjectHit = cObjectHit;
			hObj.pLocation.x = iBullet->location.x;
			hObj.pLocation.y = iBullet->location.y;
			vHits.push_back(hObj);
			vRemoveList.push_back(iBullet);
		}
		//x = iBullet->location.x/50;
		//y = iBullet->location.y/50;
		//if (x > 1) x = 1;
		//if (x < -1) x = -1;
		//if (y > 1) y = 1;
		//if (y < -1) y = -1;
	}
	
	//delete bullets that have destroyed something
	for (vRemoveListIt = vRemoveList.begin(); vRemoveListIt != vRemoveList.end(); vRemoveListIt++) {
		vBullets.erase(*vRemoveListIt);
	}
	vRemoveList.clear();
}

char TGame::doHitTest(int x, int y) {
	//do hit test
	
	if(Tank1.hitTest(x,y,2,2))//could be for dynamic bullet size
	{
		//delete &Tank1
		Tank1.isHit = true;
		return '1';
	}

	if(Tank2.hitTest(x,y,2,2))//could be for dynamic bullet size
	{
		//delete &Tank2
		Tank2.isHit = true;
		return '2';
	}

	for(int i=0;i<vObs.size();i++)
	{
		 Obstacle O = (Obstacle) vObs[i];
		 int w,h;
		 w = .5;
		 h = .5;
		 if(O.hitTest(x,y,w,h))
		 return 'w';
	}

	

	return 'n';
}

void TGame::moveTank1(char direction) {
	
	bool canMove = true;
	point tm;
	tm = Tank1.getMovePoint(direction);
	if(!Tank2.hitTest(tm.x,tm.y,Tank1.width,Tank1.height))//check contact with opposing tank
	{
	  for(int i=0;i<vObs.size();i++)
	  {
		 Obstacle O = (Obstacle) vObs[i];
		 int x,y,w,h;
		 point p = Tank1.getMovePoint(direction);
		 x= p.x;
		 y= p.y;
		 w = Tank1.width;
		 h = Tank1.height;
		 if(O.hitTest(x,y,w+2,h+2))
		  canMove = false;
	  }
	}
	else{canMove = false;}
	
	  if(canMove)
	  Tank1.move(direction);
	  
}

void TGame::moveTank2(char direction) {
	bool canMove = true;

   	point tm;
	tm = Tank2.getMovePoint(direction);
	if(!Tank1.hitTest(tm.x,tm.y,Tank2.width,Tank2.height))//check contact with opposing tank
	{
	  for(int i=0;i<vObs.size();i++)
	  {
		 Obstacle O = (Obstacle) vObs[i];
		 int x,y,w,h;
		 point p = Tank2.getMovePoint(direction);
		 x= p.x;
		 y= p.y;
		 w = Tank1.width;
		 h = Tank1.height;
		 if(O.hitTest(x,y,w+2,h+2))
		  canMove = false;
	  }
	}
	else{canMove = false;}
	
	  if(canMove)
	  Tank2.move(direction);
	
}

void TGame::rotateTank1(char direction) {
	Tank1.rotate(direction);
}

void TGame::rotateTank2(char direction) {
	Tank2.rotate(direction);
}

void TGame::fireBullet(int tankNumber) {
	bullet newBullet;
	bool fired;
	if (tankNumber == 1) {
		newBullet = Tank1.fire(&fired);
	}
	else {
		newBullet = Tank2.fire(&fired);
	}
	if (fired == true) {
		vBullets.push_back(newBullet);
	}
}