#include "rectangle.h"

Rectangle::Rectangle(int xP,int yP, int w, int h)
{
   x = xP;
   y = yP;
   width = w;
   height = h;
}

Rectangle::~Rectangle()
{
}

int Rectangle::getX()
{
  return x;
}
int Rectangle::getY()
{
  return y;
}
int Rectangle::getWidth()
{
  return width;
}
int Rectangle::getHeight()
{
  return height;
}

