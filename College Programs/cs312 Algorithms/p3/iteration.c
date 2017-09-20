/******************************************************************************
* dynamic.c - cruddy implementation of parts of Dynamic project.
* cc dynamic.c -Aa -o dynamic
*********************************************/

#include <fcntl.h>
#include <stdlib.h>
#include       <stdio.h>   /* FILE*/
#include       <string.h>  /* strcpy*/

#define        MAX_CITIES		(17)
#define        TWO_TO_MAX_CITIES        (131072)
#define        UNSET                    (65535)
#define        CAP_A                    (65)
#define        TRUE                     (1)
#define        FALSE                    (0)

typedef        unsigned short           FRAM;
typedef        unsigned long            BEAVIS;
FRAM           data_matrix[MAX_CITIES][MAX_CITIES];
FRAM           g[MAX_CITIES][TWO_TO_MAX_CITIES];
FRAM           loadfile(char filename[]);
BEAVIS         tsp();
void           debug_data_matrix(void);
void           initialize_g();
FRAM           num_cities;
BEAVIS         power2[18] = {1,2,4,8,16,32,64,128,256,512,1024,2048,4096,8192,16384,32768,65536,131072};

/*===============================================
main
================================================*/
int main(int argc, char *argv[]){
  FRAM i;
  BEAVIS set;
  BEAVIS min_dist;
  set=0;
  if (argc            < 2){
    printf ("Usage: dynamic <filename>\n");exit (1);
 }

  num_cities = loadfile (argv[1]);

  initialize_g();
  min_dist = tsp();

  printf("Data File: %s  Minimal Distance: %lu\n", argv[1], min_dist);
return 0;
}


/*===============================================
initialize_g
================================================*/
void initialize_g(){
FRAM              i;
  for (i=1; i < num_cities; i++){
    g[i][0] = data_matrix[i][0];
  }
}


/*TSP*/
BEAVIS tsp(){
  BEAVIS min_dist,temp_dist,set;
  FRAM i,j;
  min_dist = UNSET;
  for(set = 2; set < power2[num_cities]; set++){
    if (!(power2[0] & set)){
      for(i = 1; i < num_cities; i++){
        if (!(power2[i] & set)){
          min_dist = UNSET;
          for(j = 1; j < num_cities; j++){
            if (power2[j] & set){
              temp_dist = data_matrix[i][j] + g[j][set - power2[j]];
              if(temp_dist < min_dist)
                min_dist = temp_dist;
            }
          }/*city index loop*/
          g[i][set] = min_dist;
        }/*curr city in set*/
      }/*cities loop*/
    }/*A is not in set*/
  }/*set loop*/

  set = power2[num_cities]-2;
  min_dist = UNSET;
  for(j = 1; j < num_cities; j++){
    temp_dist = data_matrix[0][j] + g[j][set - power2[j]];
    if(temp_dist < min_dist)
      min_dist = temp_dist;
  }/*city index loop*/
  return min_dist;
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
FRAM loadfile(char filename[]){
  int fp ;
  char * Buffer;
  FRAM z, numlines, sum,x, k, filesize;
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
  z                      =  atol(ptr);
  sum                    =  sum +z;
  data_matrix[x][0]      = z;
  for (k=1; k < numlines; k++){
    ptr                  = strtok(NULL, " \n");
    z                    = atol(ptr);
    data_matrix[x][k]   = z;
    sum                  = sum + z;
  }
  ptr                    =  strtok(NULL, " \n");
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



