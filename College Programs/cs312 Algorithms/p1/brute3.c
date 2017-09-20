/*
CS 312 - Closest Pair Project
Group Name: Diversity
Group Members: Marcelo M., Bryan H., Carl M.
Complied under cc brute.c -o brute -lm
*/
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define MAXPOINTS  1000001
#define group_name "Diversity"
#define d
#define debug1
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
Point* points[MAXPOINTS];
Point*  ypoints[MAXPOINTS];

/*FUNCTIONS*/
unsigned long loadfile(char filename[]);
double distance(Point* p1, Point* p2);
double finaldistance(Point p1, Point p2);
Pair* do_brutally(Point* npoints[], unsigned long, unsigned long);
int compare_x(const void* p, const void* q);
int compare_y(const void* p, const void* q);
void output(char filename[], double min_dist, Point point1, Point point2);
Pair *DandC(unsigned long, unsigned long, Point*[], unsigned long);
void combine(Pair *, unsigned long , Point*[], unsigned long);

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

   printf("The number of line is %lu\n", numpoints);
   #ifdef debug1
  for (mine = 0; mine < numpoints; mine++) {
	printf("The x-array (%lu, %lu)\n", points[mine]->x, points[mine]->y);
 }
 for (mine = 0; mine < numpoints; mine++) {
	printf("The yarray (%lu, %lu)\n", ypoints[mine]->x,ypoints[mine]->y);
}
#endif
  qsort(points, (size_t)numpoints,(size_t)sizeof(points), compare_x);
  qsort(ypoints, (size_t)numpoints, (size_t)sizeof(ypoints), compare_y);

  #ifdef debug1
  for (mine = 0; mine < numpoints; mine++) {
	printf("The x-array (%lu, %lu)\n", points[mine]->x, points[mine]->y);
 }
 for (mine = 0; mine < numpoints; mine++) {
	printf("The yarray (%lu, %lu)\n", ypoints[mine]->x,ypoints[mine]->y);
}
#endif
  	
  pair = DandC(0, numpoints-1, ypoints, numpoints);



  time( &finish );
  elapsed_time = difftime( finish, start );

  printf("\n***********Using DandC******************************************\n");
  output(filename, finaldistance(pair->p1, pair->p2), pair->p1, pair->p2);
  printf( "\nProgram takes %f seconds.\n", elapsed_time );
  printf("\n*****************************************************\n\n");

  return(1);
}/* main */


Pair *DandC(unsigned long s, unsigned long e, Point *currY[], unsigned long sizey) {
	Pair *p1, *p2;
        Point *temp1[sizey], *temp2[sizey];
        int mine = 0;

	unsigned long arrayBound = 0, count = 0, itemp1 = 0, itemp2 = 0;	
	arrayBound = s + (e-s)/2;	
	#ifdef d
        printf("*************************We are sorting the arrays now************************************\n");
   		 printf("Array bound is %lu\n", arrayBound);
   		 printf("Array size is %lu\n", sizey);   		
   		 printf("Lower bound is %lu\n", s);
   		 printf("Upper bound is %lu\n", e);
        #endif
 	if ((e - s + 1) <= 3) {
		p1 = do_brutally(points,s,e);
		#ifdef d
		printf("Do brutally returned this as the minimum distance %f\n", p1->dist);
		#endif
		return p1;
	}
   else {
                for (count = 0; count < sizey; count++) {
			if (currY[count]->x <= points[arrayBound]->x) {
				temp1[itemp1] = currY[count];
				itemp1++;
			}
			else {
				temp2[itemp2] = currY[count];
				itemp2++;
			}
                }

                #ifdef debug1
                 for (mine = 0; mine < itemp1; mine++) {
	printf("The y1array (%lu, %lu)\n", temp1[itemp1]->x,temp1[itemp1]->y);
}
 for (mine = 0; mine < itemp2; mine++) {
	printf("The y2array (%lu, %lu)\n", temp2[itemp2]->x,temp2[itemp2]->y);
}
                #endif
		p1 = DandC(s, arrayBound, temp1, itemp1);
		p2 = DandC(arrayBound+1, e, temp2, itemp2);
		#ifdef d
printf("*************************Going into Combine************************************\n");		
printf("The distance for p1 is %f\n", p1->dist);
printf("The distance for p2 is %f\n", p2->dist);
#endif
		if (p1->dist < p2->dist) {
			combine(p1, arrayBound, currY,sizey);
			#ifdef d
			printf("Combined returned this as the minimum distance %f\n", p1->dist);
			#endif
			return p1;
		}
 		else {
 			combine(p2, arrayBound, currY, sizey);
 			#ifdef d
			printf("Combined returned this as the minimum distance %f\n", p2->dist); 			
			#endif
 			return p2;
 		}
   }
}
void combine(Pair *pr, unsigned long arrayBound, Point* yarray[],unsigned long yarraysize) {
         Point* ytemp[yarraysize];
         Pair *p;
	unsigned long count, ytempSize = 0, tempCount = 0;
	double temp = 0;
	int mine;
	
		for (count = 0; count < yarraysize; count++) {
			if (yarray[count]->x > points[arrayBound]->x) {
				if (yarray[count]->x <= (points[arrayBound]->x + pr->dist)) {
					ytemp[ytempSize] = yarray[count];
					ytempSize++;
			 	}
	 		}
			else if (yarray[count]->x <= points[arrayBound]->x) {
		 		if (yarray[count]->x >= (points[arrayBound]->x - pr->dist)) {
					ytemp[ytempSize] = yarray[count];
					ytempSize++; 		
			 	}
	 		}
	
		} 	//end of for
		#ifdef d
		printf("*************************************In Combine*******************************\n");
		printf("The array size is %lu\n", ytempSize);
		  for (mine = 0; mine < ytempSize; mine++) {
	printf("The y-array (%lu, %lu)\n", yarray[mine]->x, yarray[mine]->y);
				
 }
 printf("The  distance is %f\n", pr->dist);
 #endif

		
	/*	p = do_brutally(yarray, 0, yarraysize -1);
	if (p->dist < pr->dist) {
		pr->dist = p->dist;
		pr->p1.x = p->p1.x;
		pr->p1.y = p->p1.y;
		pr->p2.x = p->p2.x;
		pr->p2.y = p->p2.y;
	}
	return;*/
		
		for (count = 0; count < ytempSize; count++) {
			for (tempCount = count + 1; tempCount < ((count + 8) <= ytempSize ? count + 8 : ytempSize); tempCount++) {
				temp = distance(ytemp[count], ytemp[tempCount]);
				#ifdef d
				printf("The counter is %lu\n", tempCount);
				printf("The distance function returned %f\n");
				#endif
				if (temp < pr->dist) {
					pr->dist = temp;
					pr->p1.x = ytemp[count]->x;
					pr->p1.y = ytemp[count]->y;
					pr->p2.x = ytemp[tempCount]->x;
					pr->p2.y = ytemp[tempCount]->y;
	
				}
				#ifdef d
				printf("The smallest distance is %f\n", pr->dist);
				#endif
	 		}
	 		#ifdef d
	 		printf("Upon exiting the for loop the smallest distance is %f\n", pr->dist);
 		        #endif
 		}
 		return;/**/
 		
}


/*COMPARE_X*/
int compare_x(const void* p, const void* q)
{

  Point *p1 = *(Point**)(p);
  Point *p2 = *(Point**)(q);
  printf("We are comparing\n");
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
  Point *p1 = *(Point**)(p);
  Point *p2 = *(Point**)(q);
  if (p1->y < p2->y)
    return(-1);
  else
    if (p1->y == p2->y)
      return(0);
    else
      return(1);
}/*compare_y*/


/*DO_BRUTALLY*/
Pair* do_brutally(Point* npoints[], unsigned long s, unsigned long e)
{
  int  i, j;
  double temp ;
  Pair *pair = malloc(sizeof(Pair));
  pair->dist = 999999999;
  for (i = s; i < e; i++)
    for (j=i+1; j <= e; j++){
       temp = distance(npoints[i],npoints[j]);
       if (temp < pair->dist)
       {
              pair->dist = temp;
              pair->p1.x = npoints[i]->x;
              pair->p2.x = npoints[j]->x;
              pair->p1.y = npoints[i]->y;
              pair->p2.y = npoints[j]->y;

       }/*if*/
    }
  return pair;
}


/*DISTANCE*/
double distance(Point* p1, Point* p2)
{
  double d1,d2;
  double result;
  d1 = ((double)p1->x - (double)p2->x);
  if (d1 < 0)
    d1 = 0 - d1;
  d2 = ((double)p1->y - (double)p2->y);
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
  lseek(fp,0,SEEK_SET);
  read(fp,Buffer,filesize);
  Buffer[filesize] = 0;

  i=0;
  x=0;
  state=0;
  numlines =0;
  while(1){
    ch = Buffer[i];
    if(ch >= '0' && ch <= '9'){
      num_ptr[x] = ch;/* I can just cast these as longs latter*/
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
      num_ptr[x] = ch;/* I can just cast these as longs latter*/
      x++;
    }
    else
      if(state==0){ /* && Buffer[i+1] >='0' && Buffer[i+1] <='9'){ */
        num_ptr[x] = 0; /* save x here */
        points[numlines] = malloc(sizeof(Point*));
        points[numlines]->x =(unsigned long) atol(num_ptr);
        state =1;
        num_ptr[0]=0;
        x=0;
      }
      else
      if(state == 1){ /*  && Buffer[i+1] >='0' && Buffer[i+1] <= '9'){*/
        num_ptr[x]=0;
        points[numlines]->y=(unsigned long) atol(num_ptr);
        state=0;
        num_ptr[0]=0;
        x=0;
        ypoints[numlines] = malloc(sizeof(Point*));
        ypoints[numlines]->x = points[numlines]->x;
        ypoints[numlines]->y = points[numlines]->y;
        numlines++;
      }
    i++;
  }/* while */

  //printf("Buffer length = %lu",strlen(Buffer));

/*
  if (!(fscanf(fp, " %lu %lu ", &numlines, &true_sum)))
  if (!(fscanf(fp, " %lu ", &numlines)))
  {
    printf("\nError: missing number of lines or sum in file %s\n", filename);
    exit(1);
  }
*/

  if (numpoints==0)
  {
    printf("\nEmpty files are useless\n");
    exit(1);
  }


  if (numpoints != numlines)
  {
    printf("\nError: file lines don't match in %s\n", filename);
    printf("Specified # of lines: %d\nActual # of lines: %d\n",numlines, numpoints);
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

  if (close(fp))
  {
    printf("\nError: file %s won't close\n", filename);
    exit(1);
  }

  return(numlines);
}/* loadfile */
