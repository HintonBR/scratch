#ifndef Rectangle_1
#define Rectangle_1

class Rectangle
{
  public:
	
	int x, y, width, height;
	Rectangle(int xP, int yP, int w, int h);
	~Rectangle();
	int getX();
	int getY();
	int getWidth();
	int getHeight();

  protected:
};//end rectangle Class Header	
#endif