// bullet.h: interface for the bullet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BULLET_H__5B70A5F7_75A0_4A82_B4C0_D7612EA66B9C__INCLUDED_)
#define AFX_BULLET_H__5B70A5F7_75A0_4A82_B4C0_D7612EA66B9C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "point.h"
#include <time.h>
#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/timeb.h>

class bullet  
{
public:

	bullet();
	virtual ~bullet();
	point location;
	int angle;
	struct _timeb lastMoveTime;
	int moveSpeed;
	void update();
	bool isValidTime(int, struct _timeb);
};

#endif // !defined(AFX_BULLET_H__5B70A5F7_75A0_4A82_B4C0_D7612EA66B9C__INCLUDED_)
