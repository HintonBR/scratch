/*
CS 312 - Closest Pair Project
Group Name: Diversity
Group Members: Marcelo M., Bryan H., Carl M.
Complied under cc brute.c -o brute -lm
*/
#include <sys/stat.h>
//#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
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
Point ypoints[MAXPOINTS];

/*FUNCTIONS*/
unsigned long loadfile(char filename[]);
double distance(Point p1, Point p2);
double finaldistance(Point p1, Point p2);
Pair* do_brutally(Point npoints[], unsigned long, unsigned long);
int compare_x(const void* p, const void* q);
int compare_y(const void* p, const void* q);
void output(char filename[], double min_dist, Point point1, Point point2);
void print(Point npoints[], unsigned long n);
Pair *DandC(unsigned long,unsigned long, Point[], unsigned long, const unsigned long);
void combine(Pair *, unsigned long , Point[], unsigned long);


/*MAIN*/
int main(int argc, char *argv[])
{
  char                    *filename;
  float                   time_sec;
  unsigned long           numpoints;
  time_t   start, finish;
  double   elapsed_time;
  Pair* pair;

 int mine = 0;  time( &start );
  if (argc < 2 )
  {
    printf("Usage:  reader <filename>\n");
    exit(1);
  }
  time_sec = 0;

  filename  = argv[1];
  numpoints = loadfile(argv[1]);
  
  qsort(points, (size_t)numpoints,(size_t)sizeof(Point), compare_x);
  qsort(ypoints, (size_t)numpoints, (size_t)sizeof(Point), compare_y);     

  printf("The number of line is %lu\n", numpoints);	
  pair = DandC(0, numpoints-1, ypoints, numpoints, (numpoints - 1)/2);
   printf(" /n");


/*for (mine = 0; mine < 100; mine++) {
	//printf("The x-array (%lu, %lu)\n", points[mine].x, points[mine].y);
 }
 for (mine = 0; mine < 100; mine++) {
//printf("The yarray (%lu, %lu)\n", ypoints[mine].x,ypoints[mine].y);
}*/
  time( &finish );
  elapsed_time = difftime( finish, start );

  printf("\n***********Using DandC******************************************\n");
  output(filename, finaldistance(pair->p1, pair->p2), pair->p1, pair->p2);
  printf( "\nProgram takes %f seconds.\n", elapsed_time );
  printf("\n*****************************************************\n\n");

  //print(points,10);
  return(1);
}/* main */

Pair *DandC(unsigned long s, unsigned long e, Point currY[], unsigned long sizey, const unsigned long tempArraySize) {
	Pair *p1, *p2;
	Point temp1[100], temp2[100];	
	unsigned long arrayBound = 0, count = 0, itemp1 = 0, itemp2 = 0;	
	arrayBound = tempArraySize;
	
        //printf("we are entering into DandC and arrayBound is %lu\n",e);
 	if ((e - s + 1) <= 3) {
		return do_brutally(points,s,e);;
	}
   else {
   		//Get the two sorted y-arrays
                for (count = 0; count < sizey; count++) {
			if (currY[count].x <= points[arrayBound].x) {
				temp1[itemp1].x = currY[count].x;
				temp1[itemp1].y = currY[count].y;
				itemp1++;
			}
			else {
				temp2[itemp2].x = currY[count].x;
				temp2[itemp2].y = currY[count].y;
				itemp2++;
			}
                }
		p1 = DandC(s, arrayBound, temp1, itemp1, (s + (e-s)/2));
		p2 = DandC(arrayBound+1, e, temp2, itemp2, (s + (e-s)/2));
		if (p1->dist < p2->dist) {
			combine(p1, arrayBound, currY,sizey);
			return p1;
		}
 		else {
 			combine(p2, arrayBound, currY, sizey);
 			return p2;
 		}
   }
}
void combine(Pair *pr, unsigned long arrayBound, Point yarray[],unsigned long yarraysize) {
         Point ytemp[100];
         Pair *p;
	unsigned long count, ytempSize = 0, tempCount = 0;
	double temp = 0;
	int mine;
	/*p = do_brutally(yarray, 0, yarraysize -1);
	if (p->dist < pr->dist) {
		pr->dist = p->dist;
		pr->p1.x = p->p1.x;
		pr->p1.y = p->p1.y;
		pr->p2.x = p->p2.x;
		pr->p2.y = p->p2.y;
	}
	return;*/
		for (count = 0; count < yarraysize; count++) {
			if (yarray[count].x > points[arrayBound].x) {
				if (yarray[count].x <= (points[arrayBound].x + pr->dist)) {
					ytemp[ytempSize].x = yarray[count].x;
					ytemp[ytempSize].y = yarray[count].y; 		
					ytempSize++;
			 	}
	 		}
			else if (yarray[count].x <= points[arrayBound].x) {
		 		if (yarray[count].x >= (points[arrayBound].x - pr->dist)) {
					ytemp[ytempSize].x = yarray[count].x;
					ytemp[ytempSize].y = yarray[count].y;
					ytempSize++; 		
			 	}
	 		}
	
		} 	//end of for
		printf("********************************************************************");
		for (mine = 0; mine < (sizeof(ytemp)/sizeof(Point)); mine++) {
printf("The yarray (%lu, %lu)\n", ytemp[mine].x,ytemp[mine].y);
}
		for (count = 0; count < ytempSize; count++) {
			for (tempCount = count + 1; tempCount < ((count + 8) <= ytempSize ? count + 8 : ytempSize); tempCount++) {
				if (temp = distance(ytemp[count], ytemp[tempCount]) < pr->dist) {
					pr->dist = temp;
					pr->p1.x = ytemp[count].x;
					pr->p1.y = ytemp[count].y;
					pr->p2.x = ytemp[tempCount].x;
					pr->p2.y = ytemp[tempCount].y;
	
				}
	 		}
 		}
 		return;
 		
}
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
  //printf("(%lu , %lu)\n",p1->x,p2->x);
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
Pair* do_brutally(Point npoints[], unsigned long s, unsigned long e)
{
  int  i, j;
  double temp ;
  Pair *pair = malloc(sizeof(Pair));
  pair->dist = 999999999;
  for (i = s; i < e; i++)
    for (j=i+1; j <= e; j++){
       temp =  distance(points[i],points[j]);
       if (temp < pair->dist)
       {
              pair->dist = temp;
              pair->p1.x = points[i].x;
              pair->p2.x = points[j].x;
              pair->p1.y = points[i].y;
              pair->p2.y = points[j].y;

       }/*if*/
    }
  return pair;
}


/*DISTANCE*/
double distance(Point p1, Point p2)
{
  double d1,d2;
  double result;
  d1 = ((double)p1.x - (double)p2.x);
  if (d1 < 0)
    d1 = 0 - d1;
  d2 = ((double)p1.y - (double)p2.y);
  if (d2 < 0)
    d2 = 0 - d2;
  result = d1 + d2;
  return (result);
}/*distance*/

/*DISTANCE*/
double finaldistance(Point p1, Point p2)
{
  return sqrt( ((p1.x - p2.x)*(p1.x - p2.x))+((p1.y - p2.y)*(p1.y - p2.y)) );
}/*distance*/


/*OUTPUT*/
void output(char filename[], double min_dist, Point point1, Point point2)
{
  printf("Group Name: %s\n", group_name);
  printf("File: %s\tMinimum Distance: %.2f\t", filename, min_dist);
  printf("Points: (%lu, %lu)  (%lu, %lu)\n", point1.x, point1.y, point2.x, point2.y);
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

/*LOADFILE
unsigned long loadfile(char filename[])
{
//  FILE *fp;
  unsigned long numpoints,numlines, sum, true_sum, i,x;
  int fp, filesize;
  char * Buffer;
  char num_ptr[20];
  char ch;
  int state;

  fp = open(filename, O_RDONLY);
  if (fp == -1){
    printf("Could not open input file\n");
    exit(1);
  }

  filesize = lseek(fp,0,SEEK_END);
  //  printf("\nsize = %lu\n", filesize);

  Buffer = malloc(filesize);
  memset(Buffer, 0, (size_t)filesize);
  lseek(fp,0,SEEK_SET);
  read(fp,Buffer,filesize);
  

  i=0;
  x=0;
  state=0;
  numlines =0;
  while(1){
    ch = Buffer[i];
    if(ch >= '0' && ch <= '9'){
      num_ptr[x] = ch;/* I can just cast these as longs latter
      x++;
    }
    else
     if(x > 0){
     num_ptr[x] = 0;
     numpoints = (unsigned long) atol(num_ptr);
     num_ptr[0]=0;
     x=0;
     i++;
     break;
    }
    i++;
  }

  while(i < filesize)
  {
    ch = Buffer[i];
    if(ch >= '0' && ch <= '9'){
      num_ptr[x] = ch; /* I can just cast these as longs latter
      x++;
    }
    else
      if(state==0){ 
        num_ptr[x] = 0; /* save x here 
        points[numlines].x =(unsigned long) atol(num_ptr);
        state =1;
        num_ptr[0]=0;
        x=0;
      }
      else
      if(state == 1){ 
        num_ptr[x]=0;
        points[numlines].y=(unsigned long) atol(num_ptr);
        state=0;
        num_ptr[0]=0;
        x=0;
        ypoints[numlines].x = points[numlines].x;
        ypoints[numlines].y = points[numlines].y;
        numlines++;
      }
    i++;
  }/* while 

  //printf("Buffer length = %lu",strlen(Buffer));

/*
  if (!(fscanf(fp, " %lu %lu ", &numlines, &true_sum)))
  if (!(fscanf(fp, " %lu ", &numlines)))
  {
    printf("\nError: missing number of lines or sum in file %s\n", filename);
    exit(1);
  }


  if (numpoints==0)
  {
    printf("\nEmpty files are useless\n");
    exit(1);
  }


  if (numpoints != numlines)
  {
    printf("\nError: file lines don't match in %s\n", filename);
    printf("Specified # of lines: %d\nActual # of lines: %d\n",numpoints, numlines);
    exit(1);
  }

  /*
  if (sum!=true_sum)
  {
    printf("\nError: sum doesn't match in %s\n", filename);
    printf("Specified sum: %d\nActual sum: %d\n",true_sum, sum);
    exit(1);
  }


  if (close(fp))
  {
    printf("\nError: file %s won't close\n", filename);
    exit(1);
  }

  return(numlines);
}/* loadfile */
