/* sometimes working and very crude d and c code */
/* last revised january 11, 1998*/
/* compiled under cc dandc.c -Aa -o dandc   */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

unsigned long       loadfile(
                    char filename[]);
void                dnc (
                    int start, int end, unsigned long *dist, 
                    int *pt1, int *pt2);
void                brute_force (
                    int start,int end,unsigned long *dist, int *pt1, int *pt2);
int                 qsort_aux (
                    const void *a, const void *b);
int                 y_sort (
                    const void *a, const void *b);
int                 is_small(
                    int start, int end);
void                paste(
                    unsigned long real_mid, unsigned long delta, 
                    unsigned long *dist,int *pt1,int *pt2);


#define MAXPOINTS  30000
unsigned long      points[MAXPOINTS][2];
unsigned int       y_array[MAXPOINTS];
unsigned int       y_prime[MAXPOINTS];
unsigned long      least_distance;    
unsigned long      num_in_list;
char               *filename;
unsigned long      dist;
int                j,i, pt1, pt2;
     
/************************ main**********/
int main (int argc, char *argv[]) {
filename        = argv[1];
num_in_list     = loadfile(filename);


qsort((void*)points, (size_t)num_in_list, (size_t)(2*sizeof(unsigned long)), 
       qsort_aux);  
/*
for (i=0; i<num_in_list;i++)
  { printf("i=%d x= %lu y =%lu\n", i, points[i][0], points[i][1] );}
*/

for (i = 0; i < num_in_list; i++)
    y_array[i] = i;
qsort((void*)y_array, (size_t)num_in_list, (size_t)sizeof(unsigned long), 
y_sort);	

j=0;
dnc( 0, (int)( num_in_list - 1 ), &dist, &pt1, &pt2);
printf ("dist = %lu  x1=%lu y1=%lu x2=%lu y2 =%lu\n", dist, 
        points[pt1][0],points[pt1][1],points[pt2][0],points[pt2][1] );
return(0);
}





/* qsort_aux ===============================*/
int qsort_aux (const void *a, const void *b){
unsigned long* x1 = (unsigned long*)a;
unsigned long* x2 = (unsigned long*)b;

if (*x1 > *x2)
    return 1;
if (*x1 < *x2)
    return -1;
return 0;
}

/* y_sort ===============================*/
int y_sort (const void *a, const void *b){
int *tempa, *tempb;

tempa = (int*)a;
tempb = (int*)b;

if (points[y_array[*tempa] ][1] > points[y_array[*tempb] ][1])
    return 1;
if (points[y_array[*tempa] ][1] <points[y_array[*tempb] ][1])
    return -1;
return 0;
}  

  
/***** paste ******************/
void paste(unsigned long real_mid, unsigned long delta, unsigned long *dist,int *pt1,int *pt2){
int              i,cnt;
int              p,q;
unsigned long    neg_delta, pos_delta, x, y;

neg_delta = real_mid - delta;
pos_delta = real_mid + delta;

/* select those in the delta zone */
cnt = 0;
for (i=0; i < num_in_list; i++){
    if ( (points[y_array[i] ][0] <= pos_delta)   && 
         (points[y_array[i] ][0]  >= neg_delta)  ){
	  y_prime[cnt] = y_array[i];
	  cnt++;
          }
    }

/* find closest in delta zone*/
for (p=0; p < cnt; p++){
    for (q=p+1; q < cnt; q++){
        y = points[ y_prime[q] ][1] - points[ y_prime[p] ][1];

        if (points[y_prime[q] ][0] > points[ y_prime[p] ][0])
           x = points[y_prime[q] ][0] > points[ y_prime[p] ][0];
        else
           x = points[y_prime[p] ][0] > points[ y_prime[q] ][0]; 

        if (*dist > (x+y)){
           *dist = (x+y);
           *pt1= p; *pt2=q;
           }
        }
     }
}

/* is_small ******************/
int is_small(int start, int end){
return (end - start <=3);
}  

/* dnc ******************************************/
void dnc (int start, int end, unsigned long *dist, int *pt1, int *pt2){
unsigned long   temp_left, temp_right,  delta;
int             pt1_left, pt2_left, pt1_right, pt2_right,
                pt1_delta, pt2_delta, middle;

printf("in dandc j=%d\n",j);
j++;

if (is_small(start, end)) {
   brute_force(start, end, dist, pt1, pt2);
   } 
else {
 /* divide*/
  middle = (end + start)/2;
  dnc (start,    middle, &temp_left,  &pt1_left,  &pt1_right);
  dnc (middle+1, end,    &temp_right, &pt2_right, &pt2_right);
  delta = temp_left; pt1_delta = pt1_left; pt2_delta = pt1_right;
  if (temp_right < delta)
     {delta = temp_right;pt1_delta = pt1_right; pt2_delta = pt2_right;}
  *dist = delta; *pt1 = pt1_delta; *pt2 = pt2_delta;
  paste ( (points[end][0] - points[start][0])/2, delta, dist, pt1, pt2);
  }
}

/********************** brute_force *************/
void brute_force (int start, int end, unsigned long *dist, int *pt1, int *pt2){
int            i,j;
unsigned long  x,y, temp;

temp = 4000000;

for (i=start; i <= end; i++)
    for (j=i+1; j <= end; j++){
        if (points[i][0] > points[j][0])
          x = (points[i][0] - points[j][0]);
        else
          x = (points[j][0] - points[i][0]);

        if (points[i][1] > points[j][1])
          y = (points[i][1] - points[j][1]);
        else
          y = (points[j][1] - points[i][1]);

        if ((x+y) < temp){
            *pt1    = i;            
            *pt2    = j;
            temp    = (x+y);
            }
        }
*dist = temp;
}




/* from reader.c */
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
