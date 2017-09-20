/*   
CS 312 - Closest Pair Project
Group Name: Diversity
Group Members: Marcelo M., Bryan H., Carl M.
Complied under cc brute.c -o brute -lm
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define MAXPOINTS  1000000
#define group_name "Diversity"

/*GLOBAL*/
struct point
{
  unsigned long x;
  unsigned long y;
};
typedef struct point Point;
struct pair
{
  Point p1;
  Point p2;
  double dist;
};
typedef struct pair Pair;
Point points[MAXPOINTS];


/*FUNCTIONS*/
unsigned long loadfile(char filename[]);
double distance(Point p1, Point p2);
Pair do_brutally(Point npoints[], unsigned long n);
int compare_x(const void* p, const void* q);
int compare_y(const void* p, const void* q);
void output(char filename[], double min_dist, Point point1, Point point2);
void print(Point npoints[], unsigned long n);
//Pair closestpair(unsigned long PL[][], int n);


/*MAIN*/
int main(int argc, char *argv[])
{
  char                    *filename;
  float                   time_sec;
  unsigned long           numpoints;
  Pair pair;
  time_t   start, finish;
  double   elapsed_time;

  time( &start );	
  if (argc < 2 )
  {
    printf("Usage:  reader <filename>\n");
    exit(1);
  }
  time_sec = 0;

  filename  = argv[1];
  numpoints = loadfile(argv[1]);

  qsort(points, numpoints, sizeof(Point), compare_x);
  //print(points,numpoints);
  pair = do_brutally(points,numpoints);

  time( &finish );
  elapsed_time = difftime( finish, start );

  printf("\n****************Using Brutal*************************************\n");
  output(filename, pair.dist, pair.p1, pair.p2);
  printf( "\nProgram takes %f seconds.\n", elapsed_time );
  printf("\n*****************************************************\n\n");
  return(1);
}/* main */


/*PRINT*/
void print(Point npoints[], unsigned long n)
{
  unsigned long i;
  for(i=0; i < n; i++)
  {
    printf(" (%d, %d) \n", npoints[i].x, npoints[i].y);
  }
}


/*COMPARE_X*/
int compare_x(const void* p, const void* q)
{
  Point *p1 = (Point*)(p);
  Point *p2 = (Point*)(q);
  if (p1->x < p2->x)
    return(-1);
  else
    if (p1->x == p2->x)
      return(0);
    else
      return(1);
}/*compare_x*/


/*COMPARE_Y*/
int compare_y(const void* p, const void* q)
{
  Point *p1 = (Point*)(p);
  Point *p2 = (Point*)(q);
  if (p1->y < p2->y)
    return(-1);
  else
    if (p1->y == p2->y)
      return(0);
    else
      return(1);
}/*compare_y*/


/*DO_BRUTALLY*/
Pair do_brutally(Point npoints[],unsigned long n)
{
  unsigned int  i, j;
  double        temp ;
  Pair pair;
  pair.dist = 1000000000;
  for (i=0; i< n; i++)
    for (j=i+1; j < n; j++){
       temp =  distance(points[i],points[j]);
       if (temp < pair.dist)
       {
              pair.dist = temp;
              pair.p1.x = points[i].x;
              pair.p2.x = points[j].x;
              pair.p1.y = points[i].y;
              pair.p2.y = points[j].y;

       }/*if*/
    }
  return pair;
}


/*DISTANCE*/
double distance(Point p1, Point p2)
{
  return sqrt( ((p1.x - p2.x)*(p1.x - p2.x))+((p1.y - p2.y)*(p1.y - p2.y)) );
}/*distance*/


/*OUTPUT*/
void output(char filename[], double min_dist, Point point1, Point point2)
{
  printf("Group Name: %s\n", group_name);
  printf("File: %s\tMinimum Distance: %.2f\t", filename, min_dist);
  printf("Points: (%d, %d)  (%d, %d)\n", point1.x, point1.y, point2.x, point2.y);
}/*output*/


/*LOADFILE*/
unsigned long loadfile(char filename[])
{
  FILE *fp;
  unsigned long numlines, sum, true_sum, x;

  if (!(fp=fopen(filename, "r")))
  {
    printf("\nError: cannot open file %s\n", filename);
    exit(1);
  }

/*Begin reading file*/
/*  if (!(fscanf(fp, " %lu %lu ", &numlines, &true_sum))) */
  if (!(fscanf(fp, " %lu ", &numlines)))
  {
    printf("\nError: missing number of lines or sum in file %s\n", filename);
    exit(1);
  }

  if (numlines==0)
  {
    printf("\nEmpty files are useless\n");
    exit(1);
  }

  /*Read in coordinates*/
  x = 0; sum = 0;
  while (fscanf(fp, " %lu %lu ", &points[x].x, &points[x].y)!=EOF)
  {
    sum = sum + points[x].x + points[x].y;
    x++;
  }

  if (x!=numlines)
  {
    printf("\nError: file lines don't match in %s\n", filename);
    printf("Specified # of lines: %d\nActual # of lines: %d\n",numlines, x);
    exit(1);
  }

  /*
  if (sum!=true_sum)
  {
    printf("\nError: sum doesn't match in %s\n", filename);
    printf("Specified sum: %d\nActual sum: %d\n",true_sum, sum);
    exit(1);
  }
  */

  if (fclose(fp))
  {
    printf("\nError: file %s won't close\n", filename);
    exit(1); 
  }

  return(x);
}/* loadfile */


