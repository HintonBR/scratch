#ifndef obstacle_1
#define obstacle_1

#include "rectangle.h"

class Obstacle
{
	public:
	    float x, y, width, height;
		char dir;
		Obstacle();
        Obstacle(float xP, float yP, float w, float h);
        ~Obstacle();
		bool hitTest(int x, int y, float w, float h);
		float getN();
		float getE();
		float getW();
		float getS();

};//OBstacle Class Header
#endif
