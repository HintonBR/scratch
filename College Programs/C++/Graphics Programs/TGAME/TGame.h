// TGame.h: interface for the TGame class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TGAME_H__B4CF92EF_E775_4954_BA07_332FB11AB7E4__INCLUDED_)
#define AFX_TGAME_H__B4CF92EF_E775_4954_BA07_332FB11AB7E4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include <list>
#include <algorithm>
#include "hit.h"
#include "tank.h"
#include "obstacle.h"

class TGame  
{
public:
	TGame();
	virtual ~TGame();
	void ResetTanks();
	char doHitTest(int x, int y);
	void addObstacle(Obstacle O);
	void updateBullets();
	void moveTank1(char direction);
	void moveTank2(char direction);
	void rotateTank1(char direction);
	void rotateTank2(char direction);
	void fireBullet(int tankNumber);
	tank getTank1() { return Tank1;};
	tank getTank2() { return Tank2;};
	tank Tank1;
	tank Tank2;
	vector<bullet> vBullets;
	vector <Obstacle> vObs;
	vector<hit> vHits;
};

#endif // !defined(AFX_TGAME_H__B4CF92EF_E775_4954_BA07_332FB11AB7E4__INCLUDED_)
