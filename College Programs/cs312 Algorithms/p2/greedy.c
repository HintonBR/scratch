/*
cc greedy.c -o greedy -O
Group Name: KaheKahe kakai
Group Members: Marcelo Melo, Bryan Hinton, Carl Mitchell.
marcelom@cs.byu.edu
Greedy
*/

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*FUNCTIONS*/
unsigned long        find(unsigned long);
unsigned long        setUnion(unsigned long, unsigned long);
long                 peek(unsigned long);
long                 is_legal(unsigned long);
int                  leftjob(long jobnum);
int                  rightjob(long jobnum);
unsigned long        add(unsigned long newjob);
void                 prepare_data (void);
void                 greedy (void);
void                 print(unsigned long);
void                 printmax(unsigned long);
unsigned long        pop_head(void);
void                 heapify(void);
void                 adjust(unsigned long);
unsigned long        loadfile(char filename[]);
void                 print_output(char*, double);

/*GLOBAL VARIABLES*/
#define              MAXDEAD            1000000
#define              MAXJOBS            1000001
//TYPE DEFINITIONS
struct _JOBSET {
       unsigned long parent;
       unsigned long children;
       long hidden;
};
typedef struct _JOBSET JOBSET;

unsigned long	bids[MAXJOBS][2];
JOBSET time_job[MAXDEAD];

unsigned long        num_in_list      = 0;
unsigned long        heap_size        = 0;
unsigned long        currprice        = 0;
double        total_profit     = 0;
clock_t              starttime, endtime;
unsigned long maxdeadline;
unsigned long numinserted;


/*MAIN*/
int main(int argc, char *argv[]){
  if (argc            < 2){
    printf ("Usage: greedy <filename>\n");exit (1);
  }
  maxdeadline = 1;
  num_in_list = loadfile (argv[1]);
  heap_size   = num_in_list;
  prepare_data();
  greedy();
  print_output (argv[1], total_profit);
  return 0;
}/*main*/


/*PREPARE_DATA*/
void prepare_data(void){
  unsigned long i;
  heapify();
  for (i=0; i < MAXDEAD; i++){
    time_job[i].parent = i;
    time_job[i].children = 1;
    time_job[i].hidden = i;
  }
}/*prepare_data*/


/*GREEDY*/
void greedy(void){
  unsigned long i, j, n;
  long k;
  n=0;
  for(i = 0; i < num_in_list; i++){
    j = pop_head();
    k=is_legal(j);
    if (k >-1){
      add(k);
      numinserted++;
    }
    if (numinserted >= maxdeadline) break;
  }
}/*greedy*/


/*ISLEGAL*/
long is_legal(unsigned long newjob) {
  unsigned long parent;
  long hidden;
  parent = find(newjob);
  hidden = peek(parent);
  if (hidden == -1 )
    return -1;
  else
    return hidden;
}/*islegal*/

unsigned long find(unsigned long startnode) {
         if (startnode != time_job[startnode].parent) {
               time_job[startnode].parent = find(time_job[startnode].parent);
         }
         return time_job[startnode].parent;
}


unsigned long setUnion(unsigned long set1, unsigned long set2) {
         if (time_job[set1].children >= time_job[set2].children) {
            time_job[set1].children += time_job[set2].children;
            time_job[set2].parent = set1;
            return set1;
         }
         else
         if (time_job[set1].children < time_job[set2].children){
            time_job[set2].children += time_job[set1].children;
            time_job[set1].parent = set2;
            time_job[set2].hidden = time_job[set1].hidden;
            return set2;
         }
}

long peek(unsigned long node) {
         return time_job[node].hidden;
}

	

/*ADD*/
unsigned long add(unsigned long newjob) {
	int leftval = 0;
	int rightval = 0;
	unsigned long parent, parent2;
        leftval = leftjob((long)newjob - 1);	
	rightval = rightjob((long)newjob + 1);
	time_job[newjob].hidden = (long)newjob -1;	
        total_profit = total_profit + currprice;
        currprice = 0;
	switch (leftval) {
		case -1:
			switch (rightval) {
				case 1:
					time_job[newjob].hidden = -1;	
					break;
				case 2:
					parent = setUnion(newjob, find(newjob + 1));
                                        time_job[parent].hidden = -1;
					break;
			}
			break;
		case 1:
			switch (rightval) {
				case -1:
					time_job[newjob].hidden = (long)newjob-1;
					break;
				case 1:
					time_job[newjob].hidden = (long)newjob-1;
					break;
				case 2:
					parent = setUnion(newjob, find(newjob + 1));					
					time_job[parent].hidden = (long)newjob-1;
					break;
			}
			break;
		case 2:
			switch (rightval) {
				case -1:
					setUnion(find(newjob - 1),newjob);
					break;
				case 1:
					setUnion(find(newjob -1),newjob);						
					break;
				case 2:
					parent = setUnion(find(newjob -1),newjob);
					setUnion(parent, find(newjob + 1));
					break;
			}
			break;
	}//end of switch			
}/*add*/


int leftjob(long jobnum) {
	if (jobnum == -1) {
	 	return -1;
	}
	else if (time_job[jobnum].hidden == jobnum) {
	 	return 1;
	}
	else return 2;
}


int rightjob(long jobnum) {
	if (jobnum == 1000000) {
	 	return -1;
	}
	else if (time_job[jobnum].hidden == jobnum) {
	 	return 1;
	}
	else return 2;	

}


/*PRINT*/
void print(unsigned long n)
{
  unsigned long i;
  for(i=0; i < n; i++)
  {
    printf("%d. p=%lu c=%lu h=%d\n",i, time_job[i].parent, time_job[i].children, time_job[i].hidden);
  }
  printf("\n\n");
}/*print*/


/*PRINTMAX*/
void printmax(unsigned long n)
{
  unsigned long i,time;
  for(i=0; i < n; i++)
  {
    time = pop_head();
    printf(" (%lu  %lu) \n", time, currprice);
    currprice = 0;
  }
}/*printmax*/



/*HEAPIFY*/
void heapify (void){
  unsigned long   i;
  for (i = heap_size/2; i > 0; i--) {
    adjust(i);
  }
}/*heapify*/


/*ADJUST*/
void adjust (unsigned long i){
  unsigned long j = 2*i;
  unsigned long price    = bids[i][0];
  unsigned long deadline = bids[i][1];
  while (j <= heap_size){
    if ((j < heap_size) && (bids[j][0] < bids[j+1][0]))
      j++;
    if (price >= bids[j][0])
      break;
    bids[j/2][0] = bids[j][0];
    bids[j/2][1] = bids[j][1];
    j = j * 2;
  }
  bids[j/2][0] = price;
  bids[j/2][1] = deadline;
}/*adjust*/


/*POP_HEAD*/
unsigned long pop_head (void){
  unsigned long deadline;
  if (heap_size == 0)
    return 0;
  currprice = bids[1][0];
  deadline = bids[1][1];
  bids[1][0]     = bids[heap_size][0];
  bids[1][1]     = bids[heap_size][1];
  heap_size--;
  adjust(1);
  return deadline;
}/*pop_head*/

void print_output(char filename[], double max_profit){
  printf("Group Name: %s\n", "KaheKahe kakai");
  printf("Group Members: %s\t%s\t%s\n", "Marcelo Melo", "Bryan Hinton", "Carl Mitchell");
  printf("File: %s\tMaximum Profit: %.1f\n", filename, max_profit);
}

/*OUTPUT
void print_output(char *filename, double profit, float time_sec){
  printf("File: %s   Maximum Profit: %lf\n", filename, profit);
  printf("Time: %.2f seconds\n", time_sec);
}output*/


/*LOADFILE*/
unsigned long loadfile(char filename[]){
  unsigned long total_lines,numbids,numlines, sum, true_sum, i,x, filesize;
  int fp ;
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

  Buffer = malloc(filesize);
  lseek(fp,0,SEEK_SET);
  read(fp,Buffer,filesize);
  Buffer[filesize] = 0;

  i=0;
  x=0;
  state=0;
  while(1){
    ch = Buffer[i];
    if(ch >= '0' && ch <= '9'){
      num_ptr[x] = ch;
      x++;
    }
    else
     if(x > 0){
     num_ptr[x] = 0;
     total_lines = (unsigned long) atol(num_ptr);
     num_ptr[0]=0;
     x=0;
     i++;
     break;
    }
    i++;
  }
  numlines = 1;
  numbids = 1;
  while(i <= filesize)
  {
    ch = Buffer[i];
    if(ch >= '0' && ch <= '9'){
      num_ptr[x] = ch;
      x++;
    }
    else
      if(state==0 && x>0){
        num_ptr[x] = 0;
        bids[numlines][0] =(unsigned long) atol(num_ptr);
        state =1;
        num_ptr[0]=0;
        x=0;
      }
      else
      if(state == 1 && x>0){
        num_ptr[x]=0;
        bids[numlines][1] =(unsigned long) atol(num_ptr);
        if (bids[numlines][1] > maxdeadline) {
           maxdeadline = bids[numlines][1];
        }
        state=0;
        num_ptr[0]=0;
        x=0;
        if (bids[numlines][1] != 0){
	  if (bids[numlines][1] > 1000000)
            bids[numlines][1] = 1000000;
          bids[numlines][1]--;
	  numlines++;
        }
        numbids++;
      }
    i++;
  }/* while */

  numbids--;
  numlines--;

if (total_lines==0){
    printf("\nEmpty files are useless\n");
    exit(1);
    }

if (total_lines != numbids) {
    printf("\nError: file lines don't match in %s\n", filename);
    printf("Specified # of lines: %d\nActual # of lines: %d\n",numbids, total_lines);
    exit(1);
  }

  if (close(fp))
  {
    printf("\nError: file %s won't close\n", filename);
    exit(1);
  }

  free(Buffer);
  return(numlines);

}/*Loadfile*/
