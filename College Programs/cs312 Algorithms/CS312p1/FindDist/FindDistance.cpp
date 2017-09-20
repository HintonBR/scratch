#include <iostream.h>
#include <fstream.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <exception>
#include <unistd.h>

#define MAXPOINTS  100001
#define outputfile "results.txt"
/*GLOBAL*/
struct point
{
  unsigned long x;
  unsigned long y;
  double dist;
};
typedef struct point Point;


/*FUNCTIONS*/
unsigned long loadfile(char filename[], Point[]);
void writefile(unsigned long, Point[]);
void distance(Point* p1, Point* p2);
void finaldistance(Point p1, Point p2);
extern int pause();

/*MAIN*/
int main(int argc, char *argv[])
{
  char                    *filename;
  float                   time_sec = 0;
  unsigned long           numpoints, loopCount, loopCount2;
  time_t   start, finish;
  double   elapsed_time;
  Point* points = new Point[100001];

  time( &start );
   time_sec = 0;
  if (argc < 2 )
  {
    printf("Usage:  reader <filename>\n");
    exit(1);
  }


  filename  = argv[1];
  numpoints = loadfile(argv[1], points);
  
  for (loopCount = 0; loopCount < numpoints - 1; loopCount++) {
      for (loopCount2 = loopCount + 1; loopCount2 < numpoints; loopCount2++) {
          distance(&points[loopCount], &points[loopCount2]);
      }
  }
  writefile(numpoints, points);
  time( &finish );
  elapsed_time = difftime( finish, start );

  printf("\n*************************************************************");
  printf( "\n*Program takes %f seconds.                             *", elapsed_time );
  printf("\n**************************************************************\n");

//  pause();
  return(0);
}/* main */

void writefile(unsigned long numpoints, Point points[]) {
     unsigned long count = 0;
     ofstream output(outputfile, ios::out);
     if (output) {
        try {
            output << numpoints << endl;
            for (count = 0; count < numpoints; count++) {
                output.setf(ios::fixed);
                output << points[count].x << ", " << points[count].y <<  "  " << points[count].dist << endl;
            }
        }
        catch ( exception ex ) {
             printf("An error occured - Program exiting!");
        }
        output.close();
     }
     else {
        printf("File could not be opened.  Please go home now!");
     }


}
/*LOADFILE*/
unsigned long loadfile(char filename[], Point points[])
{
  ifstream inFile(filename, ios::in);
    unsigned long numlines, dataCount = 0;
   if (!inFile) {
      printf("\nError: cannot open file %s\n", filename);
    exit(1);
   }
  try {
      inFile >> numlines;
      while (inFile >> points[dataCount].x >> points[dataCount].y) {
            points[dataCount].dist = (double)999999999999999.0;
            dataCount++;
      }

  }
  catch (exception e) {
        cout << "Error - Reading in Data! - Please check your data file!" << endl;
        exit(1);
  }

  if (numlines != dataCount)  {
    printf("\nError: missing number of lines or sum in file %s\n", filename);
    printf("The number of lines a are %lu", numlines);
        printf("The number of lines a are %lu", dataCount);
    exit(1);
  }

  if (numlines==0)
  {
    printf("\nEmpty files are useless\n");
    exit(1);
  }

  inFile.close();

  return(numlines);
}/* loadfile */

/*DISTANCE*/
void distance(Point* p1, Point* p2)
{
  double d1,d2;
  double result;
  d1 = ((double)p1->x - (double)p2->x);
  if (d1 < 0)
    d1 = 0 - d1;
    d1 = pow(d1,2);
  d2 = ((double)p1->y - (double)p2->y);
  if (d2 < 0)
    d2 = 0 - d2;
    d2 = pow(d2,2);
  result = d1 + d2;
  if (result < p1->dist) p1->dist = result;
    if (result < p2->dist) p2->dist = result;
    //printf("This is the value of result - %f\n", result);
    //printf("The first point is - (%lu, %lu)\n", p1->x, p1->y);
    //printf("The second point is - (%lu, %lu)\n", p2->x, p2->y);
}/*distance*/

/*DISTANCE*/
void finaldistance(Point p1, Point p2)
{
  double temp = 0;
  temp =  sqrt( ((p1.x - p2.x)*(p1.x - p2.x))+((p1.y - p2.y)*(p1.y - p2.y)) );
  if (temp < p1.dist) p1.dist = temp;
    if (temp < p2.dist) p2.dist = temp;
}/*distance*/


