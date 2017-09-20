#include <stdio.h>
#include <time.h>       /* CLOCKS_PER_SEC*/
#include <sys/times.h>  /* tms*/

/* 
   Two timing routines are given:

   clock() returns the CPU time used by the calling process (this program).  
   The time includes both the user CPU time and the system CPU time needed 
   to support the process.  It does not include the system or user 
   CPU time from any other processes that may be running.

   times() returns a structure containing four variables.  
   Two of these variables are used to time this program.  The time 
   outputted includes both the user CPU time and the system CPU time needed to 
   support the process.  It does not include the system or user 
   CPU time from any other processes that may be running.

   Advantage of using clock(): higher resolution than times() 
   (which means it is more accurate) and it makes for simple code.  
   Disadvantage of using clock: because of higher resolution,  
   it is only good for about 72 minutes before the value wraps around.  

   Advantage of using times(): does not have a time limit (unlike clock()).  
   Disadvantage of using times: less accurate than clock() (it may be up to 
   a second off which is not a big deal for most programs) and the code 
   is a little messier.

   See the man page for times and clock for more details.
   compiled with:  CC reader.c -o reader
   updated December 24, 1997
*/

void time_kill(unsigned long z);

int main(){
clock_t            starttime, endtime; /* for clock*/

struct tms         time_buffer;                 /*for times*/
clock_t            startusertime, endusertime;  /* for times*/
clock_t            startsystime,  endsystime;   /* for times*/

/* start testing clock*/
starttime          = clock();
time_kill(1234567);/* YOUR PROGRAM WOULD REPLACE THIS function call*/
endtime            = clock();
printf("(clock)time=%.2f seconds\n",
               (float) (endtime-starttime)/CLOCKS_PER_SEC);
/* end testing clock*/


/* start testing times*/
times(&time_buffer);
startusertime      = time_buffer.tms_utime;
startsystime       = time_buffer.tms_stime;
time_kill(1234567);/* YOUR PROGRAM WOULD REPLACE THIS function call*/
times(&time_buffer);
endusertime        = time_buffer.tms_utime;
endsystime         = time_buffer.tms_stime;
printf("(times)time=%.2f seconds\n",
 (float) (endusertime+endsystime-startusertime-startsystime)/CLK_TCK);
/* end testing times*/

}


void time_kill(unsigned long z){
unsigned long x;
double y;
for (x=0; x<z; x++)  {
   y=x*x*x*x/(x+1)*(x+1)*(x+1)*(x+2) ;}
}












