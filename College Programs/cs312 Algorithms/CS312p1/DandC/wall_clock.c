/*********************************************
Demonstration of how to implement a wall clock on HP-UX.
Compile:   cc wall_clock.c
*********************************************/
#include </usr/include/sys/timeb.h>
#include <stdlib.h>
#include <stdio.h>


/*declarations*/
struct timeb    *pTimerStart, *pTimerStop;
double          DiffTime = 0;
int main(){
  short millipart;

  pTimerStart = (struct timeb *) malloc(sizeof(struct timeb));
  pTimerStop = (struct timeb *) malloc(sizeof(struct timeb));

  ftime(pTimerStart);
  /* DO SOMETHING HERE */
  ftime(pTimerStop);

  printf("starttime: %ld.%3ld sec.\n", pTimerStart->time,
                                                    pTimerStart->millitm);
  printf("stoptime:  %ld.%3ld sec.\n", pTimerStop->time,
                                                    pTimerStop->millitm);
  millipart = (pTimerStop->millitm - pTimerStart->millitm);

  DiffTime = (pTimerStop->time - pTimerStart->time) + ((float)millipart)/1000;
  printf("Total Time= %.2f sec\n", DiffTime);

  return 1;
}

