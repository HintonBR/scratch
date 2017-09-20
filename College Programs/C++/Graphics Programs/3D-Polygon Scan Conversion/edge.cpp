#include "edge.h"

edge::edge(float ypmin, float yp, float xp, float slope, float zp, float zslope, float npx, float npxincrement, float npy, float npyincrement, float npz, float npzincrement, float wpx, float wpy,float wpz, float ypwx, float ypwy, float ypwz) {
	ymax = yp;
	x = xp;
	if (slope != 0) xincrement = (float)(1/slope);
	else xincrement = 0;
	z = zp;
	zincrement = (float) zslope;
	nx = npx;
	nxincrement = npxincrement;
	ny = npy;
	nyincrement = npyincrement;
	nz = npz;
	nzincrement = npzincrement;
	wx = wpx;
	wy = wpy;
	wz = wpz;
	ywx = ypwx;
	ywy = ypwy;
	ywz = ypwz;
	ymin = ypmin;
}



