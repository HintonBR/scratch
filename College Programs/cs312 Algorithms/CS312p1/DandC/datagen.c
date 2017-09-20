/*  CS 312 data file generator
    last updated December 24, 1997
    Compiled using the following command line:
    CC datagen.c -o datagen
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAXPOINTS               1000000
#define MAX_X                   1000000
#define MAX_Y                   1000000
unsigned long                   arr[MAXPOINTS][2];

unsigned long get_unsignedlong  (char *,unsigned long);
void          openfile          (char*);
unsigned long get_numpoints     (void);
unsigned long get_choice        (void);
void          act_on_choice     (unsigned long, unsigned long);
unsigned long increasing        (unsigned long);
unsigned long get_sum           (unsigned long);
void          set_random        (void);
void          do_output         (unsigned long, char*);

int main (){
char                            filename[100];
unsigned long                   numpoints, choice;

set_random();
openfile(filename);
numpoints       = get_unsignedlong("Enter the # of points : ", MAXPOINTS);
choice          = get_choice();
act_on_choice(choice, numpoints);
do_output(numpoints, filename);
printf ("datagen finished properly with data in %s\n", filename);
return 0;
}


/*================================================*/
unsigned long get_unsignedlong  (char *info, unsigned long z){
unsigned long x;
printf(info);
scanf("%lu", &x);
if (x>z) {
   printf("ERROR: cannot be greater than %u\n", z);
   exit(1);
   }
return x;
}

/*================================================*/
void openfile(char* filename){
printf("Enter the name of the data file : ");
scanf ("%s",filename);
}

/*================================================*/
unsigned long get_choice           (void){
unsigned long        choice;  
printf("1. Set X at a constant value\n");
printf("2. Set Y at a constant value\n"); 
printf("3. Make X ascending\n"); 
printf("4. Make X descending\n");
printf("5. Make Y ascending\n");
printf("6. Make Y descending\n");
printf("7. Make both X and Y ascending\n");
printf("8. Make both X and Y descending\n");
printf("9. Make both X and Y random\n");
printf("Enter choice : ");
  
scanf("%lu",&choice);
return(choice);
}

/*================================================*/
unsigned long increasing        (unsigned long last){
return(last +1);
}

/*================================================*/
unsigned long decreasing        (unsigned long key){
return(key);
}

/*================================================*/
unsigned long get_sum           (unsigned long numpoints){
unsigned long i, sum;
for (i=0, sum=0;i<numpoints;i++) {sum= sum+ arr[i][0] + arr[i][1];}
return(sum);
}

/*================================================*/
void set_random                 (void){
time_t          t;
srand((unsigned) time(&t));
}

/*================================================*/
void act_on_choice     (unsigned long choice, unsigned long numpoints){
unsigned long   xval, yval;
unsigned long   xrange, yrange;
unsigned long   i;

switch(choice){
case 1 :  
              xval    = get_unsignedlong("Enter (constant) X value : ", MAX_X);
              yrange  = get_unsignedlong("Enter largest Y value : ",    MAX_Y);
              for(i=0;i<numpoints;i++) {
                arr[i][0]=xval;
                arr[i][1]=(unsigned long) (rand() % yrange);
              }
              break;

case 2 :  
              yval    = get_unsignedlong("Enter (constant) Y value : ", MAX_Y);
              xrange=   get_unsignedlong("Enter largest X value : ",    MAX_X);
              for(i=0;i<numpoints;i++) {
                arr[i][0]=(unsigned long) (rand() % xrange);
                arr[i][1]=yval;
              }
              break;

case 3 :  
              yrange  = get_unsignedlong("Enter largest Y value : ",    MAX_Y);
              for(i=0;i<numpoints;i++) {
                arr[i][0]=increasing(i);
                arr[i][1]=(unsigned long) (rand() % yrange);
              }
              break;

case 4 :  
              yrange  = get_unsignedlong("Enter largest Y value : ",    MAX_Y);
              for(i=0;i<numpoints;i++) {
                arr[i][0]=decreasing(numpoints-i-1);
                arr[i][1]=(unsigned long) (rand() % yrange);
              }
              break;

case 5 :  
              xrange=   get_unsignedlong("Enter largest X value : ",    MAX_X);
              for(i=0;i<numpoints;i++) {
                arr[i][0]=(unsigned long) (rand() % xrange);
                arr[i][1]=increasing(i);
              }
              break;

case 6 :  
              xrange=   get_unsignedlong("Enter largest X value : ",    MAX_X);
              for(i=0;i<numpoints;i++) {
                arr[i][0]=(unsigned long) (rand() % xrange);
                arr[i][1]=decreasing(numpoints-i-1);
              }
              break;

case 7 :  for(i=0;i<numpoints;i++) {
                arr[i][0]=increasing(i);
                arr[i][1]=increasing(i);
              }
              break;

case 8 :  for(i=0;i<numpoints;i++) {
                arr[i][0]=decreasing(numpoints-i-1);
                arr[i][1]=decreasing(numpoints-i-1);
              }
              break;

default : 
              xrange=   get_unsignedlong("Enter largest X value : ",    MAX_X);
              yrange  = get_unsignedlong("Enter largest Y value : ",    MAX_Y);
              for(i=0;i<numpoints;i++) {
                arr[i][0]=(unsigned long) (rand() % xrange);
                arr[i][1]=(unsigned long) (rand() % yrange);
              }
              break;
}  /* end of switch */

}


/*================================================*/
void do_output(unsigned long numpoints, char * filename){
FILE            *fp;
unsigned long   sum, i;

if (!(fp = fopen(filename,"w"))){
    printf("ERROR:  Cannot open %s\n", filename);
    exit(1);
    }
sum             = get_sum(numpoints);  

fprintf(fp, " %lu\n", numpoints, sum);
for (i=0;i<numpoints;i++)
    fprintf(fp,"%lu %lu\n",arr[i][0],arr[i][1]);
fclose (fp);
}
