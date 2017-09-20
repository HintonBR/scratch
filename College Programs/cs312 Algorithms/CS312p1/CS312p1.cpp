
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAXPOINTS  30000

/*GLOBAL*/
unsigned long points[MAXPOINTS][2];

unsigned long loadfile(char filename[]);


unsigned long loadfile(char filename[]){
FILE *fp;
unsigned long numlines, sum, true_sum, x;

if (!(fp=fopen(filename, "r"))){
    printf("\nError: cannot open file %s\n", filename);
    exit(1);
    }

/*Begin reading file*/
if (!(fscanf(fp, " %lu %lu ", &numlines, &true_sum)))  {
    printf("\nError: missing number of lines or sum in file %s\n", filename);
    exit(1);
    }

if (numlines==0){
    printf("\nEmpty files are useless\n");
    exit(1);
    }

/*Read in coordinates*/
x = 0; sum = 0;
while (fscanf(fp, " %lu %lu ", &points[x][0], &points[x][1])!=EOF)  {
    sum = sum + points[x][0] +points[x][1];
    x++;
    }

if (x!=numlines) {
    printf("\nError: file lines don't match in %s\n", filename);
    printf("Specified # of lines: %d\nActual # of lines: %d\n",numlines, x);
    exit(1);
  }

/*if (sum!=true_sum) {
    printf("\nError: sum doesn't match in %s\n", filename);
    printf("Specified sum: %d\nActual sum: %d\n",true_sum, sum);
    exit(1);
    }*/

if (fclose(fp)){
    printf("\nError: file %s won't close\n", filename);
    exit(1); 
    }

return(x);
}

/*================================================*/
/*  no matter what method you use to time your programs, be sure to convert 
    your time to seconds before passing it in to this procedure.
*/

void print_output(char filename[], unsigned long dist, unsigned long x1,
       unsigned long y1, unsigned long x2, unsigned long y2, float time_sec)
{
  printf("File: %s   Distance: %lu  (%lu, %lu) (%lu, %lu)\n",
			 filename, dist, x1, y1, x2, y2);
  printf("Time: %.2f seconds\n", time_sec);
}


int main(int argc, char *argv[]){
char                    *filename;
long                    x1, y1, x2, y2, tempx, tempy, temp;
float                   time_sec;
unsigned int            i, j;
unsigned long           numpoints;
double					dist;

if (argc<2){
    printf("Usage:  reader <filename>\n");
    exit(1);
    }
filename               = argv[1];
numpoints              = loadfile(argv[1]);


dist =100000000;
for (i=0; i< numpoints; i++)
  for (j=i+1; j < numpoints; j++){
     tempx =  points[i][0] - points[j][0];
     if (tempx <0) tempx = -tempx;
     tempy =  points[i][1] -   points[j][1];
     if (tempy <0) tempy = -tempy;
     if ((tempx * tempx) + (tempy * tempy) < dist) {
            dist = (tempx * tempx) + (tempy * tempy);
            x1 = points[i][0]; 
            x2 = points[j][0]; 
            y1 = points[i][1]; 
            y2 = points[j][1]; 
            }
     }

time_sec =0;
print_output(filename, dist, x1, y1, x2, y2, time_sec);
return(1);
}