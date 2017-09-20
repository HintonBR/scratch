#include "edge.h"

edge::edge(int yp, float xp, float slope) {
	ymax = yp;
	x = xp;
	if (slope != 0) xincrement = (1/slope);
	else xincrement = 0;
}



