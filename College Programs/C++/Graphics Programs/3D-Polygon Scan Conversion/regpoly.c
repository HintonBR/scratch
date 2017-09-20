#include <stdio.h>
#include <math.h>

#define TWOPI (2*3.14159265359)

void write_regular_polygon(int x0, int y0, int radius, int numverts, float theta0, int r, int g, int b, FILE *f)
{
   double dtheta = TWOPI / numverts, theta = theta0;
   int x, y;

   fprintf(f, "%d %d %d\n", r, g, b);
   fprintf(f, "%d\n", numverts);

   for(int i=0; i < numverts; i++)
   {
     x = (int)(radius * cos(theta));
     y = (int)(radius * sin(theta));
     fprintf(f, "%d %d ", x0 + x, y0 + y);
     theta += dtheta;
   }
   fprintf(f, "\n");
}

int main()
{
   FILE *f=fopen("regpoly.dat", "w");
   int x, y;

   for(x=0; x<8; x++)
     for(y=0; y<8; y++)
       write_regular_polygon(x * 100 + 50, y * 100 + 50, 45, x + 3, (TWOPI/8)*y, 36 * x, 16 * (x + y), 255 - 36 * y, f);

   return 0;
}


