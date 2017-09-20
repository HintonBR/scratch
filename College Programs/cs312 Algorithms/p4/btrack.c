/******************************************************************************
* dynamic.c - cruddy implementation of parts of Dynamic project.
* cc dynamic.c -Aa -o dynamic
*********************************************/

#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>   /* FILE*/
#include <string.h>  /* strcpy*/
#include <time.h>


#define        MAX_CITIES		(23)
#define        INFINITY                 (32001)
#define        CAP_A                    (65)

typedef        unsigned short           FRAM;
typedef        unsigned long            BEAVIS;
FRAM           data_matrix[MAX_CITIES][MAX_CITIES];
BEAVIS         loadfile(char filename[]);
void           tsp(int city, BEAVIS set, char* path, BEAVIS pathdist, int level);
FRAM           num_cities;
BEAVIS         power2[24] = {1,2,4,8,16,32,64,128,256,512,1024,2048,4096,8192,16384,32768,65536,131072,262144,524288,1048576,2097152,4194304,8388608};
BEAVIS         min_dist;
char           gpath[24];
//FRAM           cities[22] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22};
char           strCities[23] = "BCDEFGHIJKLMNOPQRSTUVW";
float          guess[23] ={1,1,1,1,1,1,1,1,1,1,.4,.4,.4,.2,.2,.2,.25,.2,.2,.2,.22,.17};
void debug_data_matrix(void);
 FRAM           copy_matrix[MAX_CITIES][MAX_CITIES];
BEAVIS         getKillValue(char* path, BEAVIS pathdist, int level); 
int           kills[23] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; 
int   killlevel=2; 
BEAVIS         prev_guess, curr_guess;
int            dotsp=1;
int started =0;
long tourcount;

/* PRIME_THE_PUMP */
void prime_the_pump(int loopcount){
  int j,index, temp, loop = 0;
  FRAM previous,current;
  BEAVIS temp_dist = 0;
  char cities[23];
  min_dist = 23*32001;
  previous = 0;
  srand(time(NULL));
  strcpy(cities, strCities);
  while(loop != loopcount){
    for(j = num_cities; j > 1; j--){
      index = rand()%j;
      current = cities[index];
      temp_dist += data_matrix[previous][current];
      temp = cities[j-1];
      cities[j-1] = current;
      cities[index] = temp;
      previous = current;
    }
    temp_dist += data_matrix[previous][0];
    temp_dist = guess[num_cities-1]*temp_dist;
    if (temp_dist < min_dist){
      min_dist = (temp_dist);
      curr_guess = min_dist;
    }
    previous = 0;
    loop++;
  }
  if (started == 0) prev_guess = curr_guess;
  started++;
//  printf("Started = %d\n", started);
  if ((started % 5) == 0) {
    curr_guess = prev_guess + (prev_guess * .07);
    min_dist = curr_guess;
  }
  else {
	curr_guess = prev_guess + (prev_guess * .05);
    min_dist = curr_guess;
  }
  //if ((started/100) > 0) {
    //curr_guess = prev_guess + 2000;
    //min_dist = curr_guess;
  //}
  if (curr_guess < prev_guess)
    dotsp = 0;
  else {
    dotsp = 1;
  prev_guess = curr_guess;
   }
}

void myrandom(){
  int j,index, temp, loop = 0;
  FRAM previous,current;
  BEAVIS temp_dist = 0;
  char cities[23];
  //min_dist = 23*32001;
  previous = 0;
  srand(tourcount);
  strcpy(cities, strCities);
  //printf("Starting with %s\n", cities);
  //while(loop != 1){
    for(j = num_cities; j > 1; j--){
      index = rand()%j;
      current = cities[index];
      temp_dist += data_matrix[previous][current];
      temp = cities[j-1];
      cities[j-1] = current;
      cities[index] = temp;
      previous = current;
    }
    temp_dist += data_matrix[previous][0];
    //temp_dist = guess[num_cities-1]*temp_dist;
    if (temp_dist < min_dist)
      min_dist = temp_dist;
      //curr_guess = min_dist;
    //}
    //previous = 0;
    //loop++;
  //}
  //printf("Myrandom returning with dist = %lu with a path of %s\n", min_dist, cities);
 }



BEAVIS greedy(BEAVIS set){
  FRAM i,j,previous,current,mincity;
  BEAVIS tempdist,mindist = 32001;
  tempdist = 0;
  previous = 0;

  for(j = 0; j < num_cities-1; j++){
    for(i = 1; i < num_cities; i++){
      if (power2[i] & set){
        if(data_matrix[previous][i] < mindist){
          mindist = data_matrix[previous][i];
          mincity = i;
          //printf("%d , %d = %lu\n",previous, mincity,mindist);
        }
      }
    }
    //printf("%d , %d = %lu\n",previous,mincity,mindist);
    //printf("set=%lu\n",set);
    set = set - power2[mincity];
    tempdist += mindist;
    previous = mincity;
    mindist = 32001;
  }

  tempdist += data_matrix[previous][0];
  return tempdist;
}




/* MAIN */
int main(int argc, char *argv[]){
  FRAM i;
  BEAVIS set=0;
  char tpath[24];
  long pathdist=0;
  tourcount = 0;
  min_dist = 23*32001;

  if (argc            < 2){
    printf ("Usage: dynamic <filename>\n");
    exit (1);
  }

  num_cities = loadfile (argv[1]);
  //debug_data_matrix();

  gpath[0]=0;
  for(i = 1; i < num_cities; i++)
    set = set + power2[i];
  pathdist = greedy(set);
  printf("Greedy = %lu\n",pathdist);
  while (pathdist < min_dist && tourcount < 5000) {
        myrandom();
        tourcount++;
  }
  printf("Prime took %d times and = %lu\n", tourcount, min_dist);
   do
  {
    prime_the_pump(2000);
    if (dotsp){
    //min_dist = atol(argv[2]);
    //printf("guess = %lu\n",min_dist);
    memset(tpath,0,24);
    tpath[0] = 'A';
    tsp(0,set, tpath, 0, 1);
    }
  }while(gpath[0] == 0);

  printf("Data File: %s  Minimal Distance: %lu and Path: %s\n", argv[1], min_dist, gpath);

// for(i = 1; i < 23; i++)
//   printf("level %d has %d kills.\n",i,kills[i]);

  return 0;
}



/* TSP */
void tsp(int city, BEAVIS set, char* path, BEAVIS pathdist, int level){
  FRAM temp_city;
  FRAM i,size;
  if (set == 0) {
    if (pathdist + data_matrix[city][0] <= min_dist){
        min_dist = pathdist + data_matrix[city][0];
        path[level] = 'A';
        strcpy(gpath, path);
    }
    return;
  }

  for(i = 1; i < num_cities; i++){
    if (power2[i] & set){
      path[level] = (char)i+CAP_A;
      if(pathdist + data_matrix[city][i] <= min_dist){
        if /*(level == killlevel)*/(level > 1 && level < num_cities){
          if (getKillValue(path,pathdist,level) <= min_dist){
            tsp( i , set - power2[i] , path,  pathdist + data_matrix[city][i], level+1);
   	    path[level+1] = 0;
          }
          //else  killlevel = level + 1;
          //  kills[level]++;
        }
        else{
          tsp( i , set - power2[i] , path,  pathdist + data_matrix[city][i], level+1);
          path[level+1] = 0;
        }
      }
    }
  }	
  return;
}


/* GETKILLVALUE */
BEAVIS getKillValue(char* path, BEAVIS pathdist, int level){
  int row,col,i,isinf;
  FRAM minrow, mincol;
  BEAVIS rowtotal, coltotal;
  minrow = INFINITY;
  mincol = INFINITY;
  rowtotal = 0;
  coltotal = 0;
  memcpy(copy_matrix,data_matrix,sizeof(data_matrix));

  for(i = 0; i < level; i++){
    row = (int)path[i] - CAP_A;
    for(col = 0; col < num_cities; col++){
      copy_matrix[row][col] = INFINITY;    /*KILL ROWS*/
    }
  }

  for(i = 1; i <= level; i++){
    col = (int)path[i] - CAP_A;
    for(row = 0; row < num_cities; row++){
      copy_matrix[row][col] = INFINITY;    /*KILL COLS*/
    }
  }

  for(row = 0; row < num_cities; row++){ /* EXAM EACH ROW */
    isinf = 0;
    for(i = 0; i < level; i++){
      if (row == (int)path[i] - CAP_A){
        isinf = 1;
        break;
      }
    }
    if (!isinf){  /* NON INFINITY ROWS */
      for(col = 0; col < num_cities; col++){
        if (copy_matrix[row][col] < minrow)
          minrow = copy_matrix[row][col];
      }
      rowtotal += minrow;
      for(col = 0; col < num_cities; col++)
        copy_matrix[row][col] = copy_matrix[row][col] - minrow; /* REDUCE ROW */
    }/* if */
    minrow = INFINITY;
  }/* for */

  for(col = 0; col < num_cities; col++){ /* EXAM EACH COLUMN */
    isinf = 0;
    for(i = 1; i <= level; i++){
      if (col == (int)path[i] - CAP_A){
        isinf = 1;
        break;
      }
    }
    if (!isinf){  /* NON INFINITY COLUMNS */
      for(row = 0; row < num_cities; row++){
        if (copy_matrix[row][col] < mincol)
          mincol = copy_matrix[row][col];
      }
      coltotal += mincol;
    }/* if */
    mincol = INFINITY;
  }/* for */

  return rowtotal + coltotal + pathdist;
}



/*===============================================
debug_data_matrix
================================================*/
void debug_data_matrix(void){
  int i, j;
  printf("\n");
  for (i=0; i < num_cities; i++){
    for (j=0; j < num_cities; j++){
      printf("%lu ", data_matrix[i][j]);
    }
    printf("\n");
  }
  printf("\n");
}


/*===============================================
loadfile (modified)
================================================*/
BEAVIS loadfile(char filename[]){
  int fp ;
  char * Buffer;
  BEAVIS z, numlines, sum,x, k, filesize;
  char*  ptr;
  x=0;

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

  ptr = strtok(Buffer, " \t");
  numlines = atol(ptr);

if (numlines==0){
    printf("\nEmpty files are useless\n");
    exit(1);
    }

x = 0; sum = 0;
ptr = strtok(NULL, " \t");
while (ptr != 0){
  z = atol(ptr);
  sum = sum +z;
  data_matrix[x][0] = z;
  for (k=1; k < numlines; k++){
    ptr = strtok(NULL, " \n");
    z = atol(ptr);
    data_matrix[x][k] = z;
    sum = sum + z;
  }
  ptr = strtok(NULL, " \n");
  data_matrix[x][x] = INFINITY;
  x++;
}


if (x!=numlines) {
    printf("\nError: file lines don't match in %s\n", filename);
    printf("Specified # of lines: %d\nActual # of lines: %d\n",numlines, x);
    exit(1);
  }


if (close(fp)){
    printf("\nError: file %s won't close\n", filename);
    exit(1); 
    }

return(x);
}


