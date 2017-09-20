// tank.h: interface for the tank class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TANK_H__ED6015A2_5CB9_11D5_BB72_00A0CC379114__INCLUDED_)
#define AFX_TANK_H__ED6015A2_5CB9_11D5_BB72_00A0CC379114__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <time.h>
#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/timeb.h>
#include <string.h>
#include "point.h"
#include "bullet.h"
using namespace std;

class tank
{
public:
	point getMovePoint(char direction);
	tank();
	tank(float x, float y, float w, float h);
	virtual ~tank();
	bool hitTest(float x, float y, float w, float h);
	bool move(char direction);
	bool isValidTime(int, struct _timeb);
	void rotate(char direction);
	bullet fire(bool*);
	
	//Bullet* Fire(); // TO DO

	float width;
	float height;
	int angle;
	int fireSpeed;
	int moveSpeed;
	point location;
	bool isHit;
	struct _timeb lastMoveTime;
	struct _timeb lastFireTime;
};

#endif // !defined(AFX_TANK_H__ED6015A2_5CB9_11D5_BB72_00A0CC379114__INCLUDED_)
