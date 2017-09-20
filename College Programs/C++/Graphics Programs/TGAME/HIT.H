// hit.h: interface for the hit class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HIT_H__5CADBA7F_333E_468A_BA84_F1E58CF3E714__INCLUDED_)
#define AFX_HIT_H__5CADBA7F_333E_468A_BA84_F1E58CF3E714__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "point.h"

class hit  
{
public:
	hit();
	virtual ~hit();
	char cObjectHit;
	point pLocation;
};

#endif // !defined(AFX_HIT_H__5CADBA7F_333E_468A_BA84_F1E58CF3E714__INCLUDED_)
