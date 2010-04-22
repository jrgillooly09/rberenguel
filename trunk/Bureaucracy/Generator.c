#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define RIGHTWALL 1
#define DOWNWALL 2
#define LEFTWALL 3
#define UPWALL 4
#define RIGHT 1
#define DOWN 2
#define LEFT 3
#define UP 4

#define TILESIZE 20

int CanMove (int where, int x, int y, int **Maze)
{
  if(Maze[x][y]==where)return 0;
  return 1;
}

int RandomMaze(int height, int width, int **Maze)
{
  int i, j, wall, count=0;
  for(i=0;i<width;i++){
    for(j=0; j<height;j++){
      wall=rand()%8;
      Maze[i][j]=(wall>4)?0:wall;
      count+=(Maze[i][j]>0);
    }
  }
  return count;
}

void Rightwall(int r){
  char rlineto[]="rmoveto", lineto[]="moveto";
  printf("\t%% Right wall\n \t %d 0 %s\n", TILESIZE,r?rlineto:rlineto);
  printf("\t0 -%d rlineto \n", TILESIZE);
  printf("\t0 %d rmoveto \n", TILESIZE);
}

void Leftwall(int r){
  char rlineto[]="rlineto", lineto[]="lineto";
  printf("\t%% Left wall\n");
  printf("\t0 -%d %s \n", TILESIZE,r?rlineto:rlineto);
  printf("\t 0 %d rmoveto\n\t %d 0 rmoveto\n", TILESIZE, TILESIZE);
}

void Upwall(int r){
  char rlineto[]="rlineto", lineto[]="lineto";
  printf("\t%% Up wall\n");
  printf("\t %d 0 %s\n", TILESIZE,r?rlineto:rlineto);
}

void Downwall(int r){
  char rlineto[]="rmoveto", lineto[]="moveto";
  printf("\t%% Down wall\n \t 0 -%d %s\n", TILESIZE,r?rlineto:rlineto);
  printf("\t %d 0 rlineto\n\t 0 %d rmoveto\n", TILESIZE, TILESIZE);
}

void EmptySpace(int r){
  char rlineto[]="rmoveto", lineto[]="moveto";
  printf("\t%% Empty\n \t%d 0 %s \n", TILESIZE,r?rlineto:rlineto);
}

void DrawMaze (int height, int width, int **Maze, int ascii)
{
  int i, j;
  printf("newpath 100 200 moveto\n");
  printf("%d 0 rlineto 0 %d rlineto \n", width*TILESIZE, height*TILESIZE);
  printf("-%d 0 rlineto 0 -%d rlineto \n", width*TILESIZE, height*TILESIZE);
  printf("stroke closepath\n\nnewpath\n 100 %d moveto\n", 200+height*TILESIZE);
  for(i=0;i<width;i++){
    for(j=0; j<height;j++){
      if(!ascii){printf("%% ");}
      printf("%d ",Maze[i][j]);
    }
    printf("\n");
  }
  for(i=0;i<width;i++){
    for(j=0; j<height;j++){
      if(!ascii){
	switch(Maze[i][j]){
	case RIGHTWALL:
	  Rightwall(i+j);
	  break;
	case LEFTWALL:
	  Leftwall(i+j);
	  break;
	case UPWALL:
	  Upwall(i+j);
	  break;
	case DOWNWALL:
	  Downwall(i+j);
	  break;
	default:
	  EmptySpace(i+j);
	  break;
	}
      }
    }
    printf("\t%%New level\n\t %d -%d rmoveto\n",-width*TILESIZE,TILESIZE);
  }
  printf("closepath stroke showpage");
  return;
}

int main(int argc, char *argv[]){
  int i,count, ** Maze, ascii;
  int width, height;
  if(argc<3){
    puts("Usage: width height ascii");
    exit(0);
  }
  width=atoi(argv[1]);
  height=atoi(argv[2]);
  ascii=atoi(argv[3]);
  Maze=(int**)malloc(height*sizeof(int*));
  for(i=0;i<width;i++){
    Maze[i]=(int*)malloc(height*sizeof(int));
  }
  srand (time(0));
  count=RandomMaze(10,10,Maze);
  DrawMaze(10,10,Maze, ascii);
  printf("\n%% Generated %d walls\n",count);
  return 0;
}
  
