// Copyright 2009 Rubén Berenguel

// ruben /at/ maia /dot/ ub /dot/ es

// This program is free software: you can redistribute it and/or
// modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.

// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
// General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program. If not, see
// <http://www.gnu.org/licenses/>.

// 20091208@16:50

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include "GestioImg.h"
#include "GestioImg.c"

int Generations=100000;
int MaximumSteps=10;
int RANDOM=1;
int RandomSwapper=50;
int RandomMover=50;
int RandomPruner=20;
#define BigMaximumSteps 5*MaximumSteps;

int JIGGLER=1;
int VERBOSE=0, DEBUG=0;
int RES = 200;
int NumTriangles = 50;
int MaxTriangles = 100;
double BaseColor[3]={0,0,0};
double GlobalChanges=0, GlobalSwap=0;
double SpeedLimit=4000;

void AverageColor(int size, double ***matrix)
{
  int i, j;
  BaseColor[0]=0;
  BaseColor[1]=0;
  BaseColor[2]=0;
  for(i=0;i<size;i++){
    for(j=0;j<size;j++){
      BaseColor[0]+=matrix[0][i][j];
      BaseColor[1]+=matrix[1][i][j];
      BaseColor[2]+=matrix[2][i][j];
    }
    BaseColor[0]/=(double)size;
    BaseColor[1]/=(double)size;
    BaseColor[2]/=(double)size;
  }
  printf("Base color: %3.0lf %3.0lf %3.0lf\n", BaseColor[0],BaseColor[1],BaseColor[2]);
  return;
}

void RandTriangle(double *TriangleSet, int Size){
  int j=0;
  double aux;
  TriangleSet[j++]=-0.25*RES+rand()/((double)(RAND_MAX)+1)*1.5*RES; // x1
  TriangleSet[j++]=-0.25*RES+rand()/((double)(RAND_MAX)+1)*1.5*RES; // y1
  aux=0.25*Size+rand()/((double)(RAND_MAX)+1)*0.5*Size;
  aux=aux>1?aux:1;
  TriangleSet[j++]=TriangleSet[0]-aux; //x2
  aux=0.25*Size+rand()/((double)(RAND_MAX)+1)*0.5*Size;
  aux=aux>1?aux:1;
  TriangleSet[j++]=TriangleSet[1]-aux; //y2
  aux=0.25*Size+rand()/((double)(RAND_MAX)+1)*0.5*Size;
  aux=aux>1?aux:1;
  TriangleSet[j++]=TriangleSet[0]-aux; //x3
  aux=0.25*Size+rand()/((double)(RAND_MAX)+1)*0.5*Size;
  aux=aux>1?aux:1;
  TriangleSet[j++]=TriangleSet[1]-aux; //y3

  TriangleSet[j++]=rand()/((double)(RAND_MAX)+1)*255.; //c1
  TriangleSet[j++]=rand()/((double)(RAND_MAX)+1)*255.; //c2
  TriangleSet[j++]=rand()/((double)(RAND_MAX)+1)*255.; //c3
  aux=0.2+1.*(rand()/((double)(RAND_MAX)+1));
  TriangleSet[j++]=aux>1?1:aux; //alpha
}

void JiggleTriangle(double *TriangleSet, double JiggleFactor){
  // We want to change the triangle parameters around the current
  // parameters, by a JiggleFactor between 0 and 1
  int j=0;
  double aux, Jiggle;
  Jiggle=JiggleFactor*RES;
  TriangleSet[0]=-0.5*Jiggle+TriangleSet[0]+Jiggle*rand()/((double)(RAND_MAX)+1);
  TriangleSet[1]=-0.5*Jiggle+TriangleSet[1]+Jiggle*rand()/((double)(RAND_MAX)+1);
  TriangleSet[2]=-0.5*Jiggle+TriangleSet[2]+Jiggle*rand()/((double)(RAND_MAX)+1);
  TriangleSet[3]=-0.5*Jiggle+TriangleSet[3]+Jiggle*rand()/((double)(RAND_MAX)+1);
  TriangleSet[4]=-0.5*Jiggle+TriangleSet[4]+Jiggle*rand()/((double)(RAND_MAX)+1);
  TriangleSet[5]=-0.5*Jiggle+TriangleSet[5]+Jiggle*rand()/((double)(RAND_MAX)+1);
  Jiggle=JiggleFactor*255;
  TriangleSet[6]=fabs(-0.5*Jiggle+TriangleSet[6]+rand()/((double)(RAND_MAX)+1)*Jiggle); //c1
  if(TriangleSet[6]>255)TriangleSet[6]=255;
  TriangleSet[7]=fabs(-0.5*Jiggle+TriangleSet[7]+rand()/((double)(RAND_MAX)+1)*Jiggle); //c2
  if(TriangleSet[7]>255)TriangleSet[7]=255;
  TriangleSet[8]=fabs(-0.5*Jiggle+TriangleSet[8]+rand()/((double)(RAND_MAX)+1)*Jiggle); //c3
  if(TriangleSet[8]>255)TriangleSet[8]=255;
  for(j=0;j<3;j++)TriangleSet[6+j]=TriangleSet[6+j]>255?255:TriangleSet[6+j];
  aux=0.2+JiggleFactor*(rand()/((double)(RAND_MAX)+1))-0.5*JiggleFactor;
  TriangleSet[9]=aux>1?1:fabs(aux); //alpha
}

void RandTrianglePosition(double *TriangleSet){
  int j=0;
  TriangleSet[j++]=-0.25*RES+rand()/((double)(RAND_MAX)+1)*1.5*RES; // x1
  TriangleSet[j++]=-0.25*RES+rand()/((double)(RAND_MAX)+1)*1.5*RES; // y1
  TriangleSet[j++]=-0.25*RES+rand()/((double)(RAND_MAX)+1)*1.5*RES; // x2
  TriangleSet[j++]=-0.25*RES+rand()/((double)(RAND_MAX)+1)*1.5*RES; // y2
  TriangleSet[j++]=rand()/((double)(RAND_MAX)+1)*RES; // x3
  TriangleSet[j++]=rand()/((double)(RAND_MAX)+1)*RES; // y3
}

void RandTriangleColors(double *TriangleSet){
  TriangleSet[6]=rand()/((double)(RAND_MAX)+1)*255.; //c1
  TriangleSet[7]=rand()/((double)(RAND_MAX)+1)*255.; //c2
  TriangleSet[8]=rand()/((double)(RAND_MAX)+1)*255.; //c3
}

void RandTriangleAlpha(double *TriangleSet){
  double aux;
  aux=0.2+1.*(rand()/((double)(RAND_MAX)+1));
  TriangleSet[9]=aux>1?1:aux; //alpha
}

void RandBlack(double *TriangleSet){
  int j=0;
  TriangleSet[j++]=-0.5*RES+rand()/((double)(RAND_MAX)+1)*2*RES; // x1
  TriangleSet[j++]=-0.5*RES+rand()/((double)(RAND_MAX)+1)*2*RES; // y1
  TriangleSet[j++]=-0.5*RES+rand()/((double)(RAND_MAX)+1)*2*RES; // x2
  TriangleSet[j++]=-0.5*RES+rand()/((double)(RAND_MAX)+1)*2*RES; // y2
  TriangleSet[j++]=rand()/((double)(RAND_MAX)+1)*RES; // x3
  TriangleSet[j++]=rand()/((double)(RAND_MAX)+1)*RES; // y3
  TriangleSet[j++]=BaseColor[0]; //c1
  TriangleSet[j++]=BaseColor[1]; //c2
  TriangleSet[j++]=BaseColor[2]; //c3
  TriangleSet[j++]=0.1; //alpha
}


void Swap2Triangle(double *TriangleSet, int pos1, int pos2){
  int i;
  double aux;
  for(i=0;i<10;i++){
    aux=TriangleSet[10*pos1+i];
    TriangleSet[10*pos1+i]=TriangleSet[10*pos2+i];
    TriangleSet[10*pos2+i]=aux;
  }
  return;
}

void MoveTriangle(double *TriangleSet, int pos1, int pos2){
  //Moves pos1 into pos2
  int i,j;
  double aux, TrianglesAux[10];
  for(i=0;i<10;i++){
    //Back it up
    TrianglesAux[i]=TriangleSet[10*pos1+i];
  }
  for(j=0;j<10;j++){
    TriangleSet[pos1*10+j]=TriangleSet[pos2*10+j];
  }
 

  for(i=0;i<10;i++){
    TriangleSet[10*pos2+i]=TrianglesAux[i];// Paste backup
  }
  return;
}

void SwapTriangle(double *TriangleSet, int dimension){
  int i, newpos1, newpos2;
  double aux;
  if(dimension<2){return;}
  newpos1=(floor(rand()%(dimension)));
  newpos2=(floor(rand()%(dimension)));
  for(i=0;i<10;i++){
    aux=TriangleSet[10*newpos1+i];
    TriangleSet[10*newpos1+i]=TriangleSet[10*newpos2+i];
    TriangleSet[10*newpos2+i]=aux;
  }
  return;
}

void RemoveTriangle(double *TriangleSet, int dimension){
  int i,j;
  if(dimension<1){return;}
  for(i=0;i<dimension-2;i++){
    for(j=0;j<10;j++){	
      TriangleSet[i*10+j]=TriangleSet[(i+1)*10+j];
    }
  }
  for(j=0;j<10;j++){
    TriangleSet[(dimension-1)*10+j]=0.;
  }
  return;
}


int Barycentric (double *Triangle, double *point, double *L)
{
  int j=0;
  double x, y, x1, x2, x3, y1, y2, y3, x13, x23, y13, y23, xm3, ym3, det;
  x1=Triangle[j++];y1=Triangle[j++];
  x2=Triangle[j++];y2=Triangle[j++];
  x3=Triangle[j++];y3=Triangle[j++];
  x=point[0];y=point[1];
  x13=x1-x3;
  x23=x2-x3;
  y13=y1-y3;
  y23=y2-y3;
  xm3=x-x3;
  ym3=y-y3;
  det= x13*y23-x23*y13;
  if(fabs(det)<1e-15){return 0;}
  j=0;
  L[j++]=(y23*xm3-x23*ym3)/det;
  L[j++]=(x13*ym3-y13*xm3)/det;
  L[j++]=1-L[0]-L[1];
  if((L[0]>0)&&(L[0]<1)&&(L[1]>0)&&(L[1]<1)&&(L[2]>0)&&(L[2]<1)){
    L[3]=Triangle[6]; //C1
    L[4]=Triangle[7];
    L[5]=Triangle[8]; //C3
    L[6]=Triangle[9]; //A
    return 1;
  }else{return 0;}
}

void GenerateMatrix(double *Triangles, double ***auxmatrix, int NumTr, int Size)
{
  double point[2], L[7];
  int i, j, k;
  for(i=0;i<Size;i++){
    for(j=0;j<Size;j++){
      auxmatrix[0][i][j]=BaseColor[0];
      auxmatrix[1][i][j]=BaseColor[1];
      auxmatrix[2][i][j]=BaseColor[2];
      for(k=0;k<NumTr;k++){
	point[0]=(double)i;
	point[1]=(double)j;
	if(Barycentric(Triangles+10*k, point, L)){
	  auxmatrix[0][i][j]=(1.-L[6])*auxmatrix[0][i][j]+L[6]*L[3];
	  auxmatrix[1][i][j]=(1.-L[6])*auxmatrix[1][i][j]+L[6]*L[4];
	  auxmatrix[2][i][j]=(1.-L[6])*auxmatrix[2][i][j]+L[6]*L[5];
	}
      }
    }
  }
  return;
}

double MatrixDistance(int size, double ***mat1, double ***mat2)
{
  int i,j, k;
  double acum=0,acumC=0, delta;
  for(i=0;i<size;i++){
    acumC=0;
    for(j=0;j<size;j++){
      delta=0;
      acumC=0;
      for(k=0;k<3;k++){
	delta=mat1[k][i][j]-mat2[k][i][j];
	acum+=delta*delta; 
      }

    }
  }
  return sqrt(acum);
}

double GrowthFactor(int TriangleCount)
{
  double aux;
  //aux=0.995;
  aux=0.999+TriangleCount/((double)MaxTriangles)*(0.000999);
  //aux=TriangleCount<6?(0.75+TriangleCount/5*0.25):1;
  return aux;
}
    
int TriangleAdder(double *Triangles, double *Fit, int *TriangleCount, double ***matrix, double ***image, int flag){
  // Gets a set of triangles and pointers to actual fit and actual
  // count. Also the main image matrix and main new image matrix, to
  // avoid new allocations Assumes there is a MaxTriangles globally
  // defined. Tries to add a random triangle on top, improving the
  // image linearly, according to some growth parameters. If
  // TriangleCount is the maximal number of triangles, returns 0, if
  // in the maximal number of tries a triangle is generated, return 1.
  // It adds triangles of smaller sizes, as we are closer to
  // MaxTriangles.
      
  int i,steps, LocalTriangleCount;
  double *TrianglesChild, NewFit=0, Size;
  TrianglesChild=(double*)malloc(MaxTriangles*10*sizeof(double));
  if(TrianglesChild==NULL){puts("Error allocating memory");exit(3);}
  steps=-1;
  for(i=0;i<10*MaxTriangles;i++){
    TrianglesChild[i]=Triangles[i];
  }
  LocalTriangleCount=*TriangleCount+1;
  if(*TriangleCount<NumTriangles){
    do{
      steps++;
      if(steps>2*MaximumSteps){free(TrianglesChild); return 0;}
      Size= RES-(*TriangleCount/(1.*MaxTriangles))*(RES-0.7*RES);
      //Size=RES;
      RandTriangle(TrianglesChild+10*(*TriangleCount),Size);
      GenerateMatrix(TrianglesChild,matrix, LocalTriangleCount, RES);
      NewFit=MatrixDistance(RES, matrix, image);
      if(VERBOSE){
	printf("ADDING: \t\tStep: %d \tF1: %6.3lf \tF2: ",steps,*Fit);
	printf("%6.3lf \t#Tr: %d\n", NewFit,LocalTriangleCount);
      }
    }while(NewFit>GrowthFactor(*TriangleCount)*(*Fit));
	
    //If we get here, there is a new good approximation. Rewrite triangles
    *Fit=NewFit;
    for(i=0;i<10*MaxTriangles;i++){
      Triangles[i]=TrianglesChild[i];
    }
    *TriangleCount=LocalTriangleCount;
    free(TrianglesChild);
    return 1;
  }
  free(TrianglesChild);
  return 0;
}

int TrianglePruner(double *Triangles, double *Fit, int *TriangleCount, double ***matrix, double ***image){
  // Tries to prune every possible triangle
      
  int i,m, LocalTriangleCount;
  double *TrianglesChild, NewFit=0;
  TrianglesChild=(double*)malloc(MaxTriangles*10*sizeof(double));
  if(TrianglesChild==NULL){puts("Error allocating memory");exit(3);}
  LocalTriangleCount=*TriangleCount;
  if(LocalTriangleCount<1){
    free(TrianglesChild);
    return 0;}
      
  for(m=0;m<LocalTriangleCount;m++){
    for(i=0;i<10*MaxTriangles;i++){
      TrianglesChild[i]=Triangles[i];
    }
    RemoveTriangle(TrianglesChild+10*m, LocalTriangleCount-m);
    GenerateMatrix(TrianglesChild,matrix, LocalTriangleCount-1, RES);
    NewFit=MatrixDistance(RES, matrix, image);
    if(VERBOSE){
      printf("PRUNING: \tTr %d\tF1: %6.3lf \tF2: ",m,*Fit);
      printf("%6.3lf \t#Tr: %d\n", NewFit,LocalTriangleCount-1);
    }
    if(NewFit<=*Fit){
      for(i=0;i<10*MaxTriangles;i++){
	Triangles[i]=TrianglesChild[i];
      }
      *Fit=NewFit;
      *TriangleCount=LocalTriangleCount-1;
      free(TrianglesChild);
      return 5;
    }
  }
  free(TrianglesChild);
  return 0;
}

int TriangleSwapper(double *Triangles, double *Fit, int *TriangleCount, double ***matrix, double ***image){
  // Tries to swap every possible pair of triangles
      
  int i,m,n,steps;
  double *TrianglesChild, NewFit=0;
  TrianglesChild=(double*)malloc(MaxTriangles*10*sizeof(double));
  if(TrianglesChild==NULL){puts("Error allocating memory");exit(3);}

  steps=-1;
      
  if(*TriangleCount<2){
    free(TrianglesChild);
    return 0;}
      

  for(m=0;m<*TriangleCount-1;m++){
    for(n=m+1;n<*TriangleCount;n++){
      for(i=0;i<10*MaxTriangles;i++){
	TrianglesChild[i]=Triangles[i];
      }
      Swap2Triangle(TrianglesChild, m,n);
      GenerateMatrix(TrianglesChild, matrix, *TriangleCount, RES);
      NewFit=MatrixDistance(RES, matrix, image);
      if(VERBOSE){
	printf("SWAPPING: \tTr %d & %d \tF1: %6.3lf \tF2: ",n,m,*Fit);
	printf("%6.3lf \t#Tr: %d\n", NewFit,*TriangleCount);
      }
      if(NewFit<*Fit){
	for(i=0;i<10*MaxTriangles;i++){
	  Triangles[i]=TrianglesChild[i];
	}
	*Fit=NewFit;
	free(TrianglesChild);
	return 3;
      }
    }
  }
  free(TrianglesChild);
  return 0;
}    

int TriangleMover(double *Triangles, double *Fit, int *TriangleCount, double ***matrix, double ***image){
  // Tries to swap every possible pair of triangles
      
  int i,m,n,steps;
  double *TrianglesChild, NewFit=0;
  TrianglesChild=(double*)malloc(MaxTriangles*10*sizeof(double));
  if(TrianglesChild==NULL){puts("Error allocating memory");exit(3);}

  steps=-1;
      
  if(*TriangleCount<2){
    free(TrianglesChild);
    return 0;}
      

  for(m=0;m<*TriangleCount;m++){
    for(n=0;n<*TriangleCount;n++){
      if(n==m){continue;}
      for(i=0;i<10*MaxTriangles;i++){
	TrianglesChild[i]=Triangles[i];
      }
      MoveTriangle(TrianglesChild, m,n);
      GenerateMatrix(TrianglesChild, matrix, *TriangleCount, RES);
      NewFit=MatrixDistance(RES, matrix, image);
      if(VERBOSE){
	printf("MOVING: \tTr %d & %d \tF1: %6.3lf \tF2: ",n,m,*Fit);
	printf("%6.3lf \t#Tr: %d\n", NewFit,*TriangleCount);
      }
      if(NewFit<*Fit){
	for(i=0;i<10*MaxTriangles;i++){
	  Triangles[i]=TrianglesChild[i];
	}
	*Fit=NewFit;
	free(TrianglesChild);
	return 4;
      }
    }
  }
  free(TrianglesChild);
  return 0;
}    

int TriangleChanger(double *Triangles, double *Fit, int *TriangleCount, double ***matrix, double ***image, int flag)
{
  // Tries to change all triangles MaximumSteps or BigMaximumSteps
  // (flag 0, flag1) improving image quality
  int i, j,k, steps, escaped=0, localMaxSteps, Jiggle=0;  
  double *TrianglesChild, NewFit=1e6;
  localMaxSteps=flag?(rand()%MaximumSteps):(rand()%MaximumSteps);
  TrianglesChild=(double*)malloc(MaxTriangles*10*sizeof(double));
  if(TrianglesChild==NULL){puts("Error allocating memory");exit(3);}

  k=rand()%*TriangleCount;
  for(i=0;i<10*MaxTriangles;i++){
    TrianglesChild[i]=Triangles[i];
  }
  for(j=0;j<(rand()%(*TriangleCount+1));j++){
    steps=-1;
    // Moved the "copy piece"
    do{
      escaped=0;
      steps++;
      if(steps>localMaxSteps){escaped=1;break;}
      if(JIGGLER){
        Jiggle=1;JiggleTriangle(TrianglesChild+10*k,0.1**Fit/SpeedLimit);}
      else{Jiggle=0;
	if(rand()%3==0){RandTrianglePosition(TrianglesChild+10*k);}
	else{
//	  if(rand()%10==0){Jiggle=1;JiggleTriangle(TrianglesChild+10*k,*ActualFit/25000.);}else{}
	  if(rand()%5==0){RandTriangleAlpha(TrianglesChild+10*k);}else{
	    if(rand()%4==0){RandTriangle(TrianglesChild+10*k, 0.5*RES);
        }else{
	      RandTriangleColors(TrianglesChild+10*k);}}}}
      GenerateMatrix(TrianglesChild, matrix, *TriangleCount, RES);
      NewFit=MatrixDistance(RES, matrix, image);
      if(NewFit>=*Fit){
	for(i=0;i<10;i++){
	  TrianglesChild[i+10*k]=Triangles[i+10*k];
	}
      } 
      if(VERBOSE){
	if(Jiggle){
	  printf("JIGGLING: \tTr %d \tStep: %d \tF1: %6.3lf \tF2: ",k, steps,*Fit);
	  printf("%6.3lf \t#Tr: %d\n", NewFit,*TriangleCount);
	}else{
	  printf("CHANGING: \tTr %d \tStep: %d \tF1: %6.3lf \tF2: ",k, steps,*Fit);
	  printf("%6.3lf \t#Tr: %d\n", NewFit,*TriangleCount);}
      }
    }while(NewFit>=*Fit);
    k=k+1;
    k=k%*TriangleCount;
    if(!escaped){
      //If we get here, there is a new good approximation. Rewrite triangles
      *Fit=NewFit;
      for(i=0;i<10*MaxTriangles;i++){
	Triangles[i]=TrianglesChild[i];
      }
      free(TrianglesChild);
      return Jiggle?6:2;
    }
  }
  // We have tried to change all triangles unsuccessfully
  free(TrianglesChild);
  JIGGLER=!JIGGLER;
  return 0;
}
  
void NormaliseTriangles(double *Triangles, double *Normalised, int TriangleCount)
{
  int i,j=0;
  for(i=0;i<TriangleCount;i++){
    // Normalised points
    for(j=0;j<6;j++){
      Normalised[10*i+j]=Triangles[10*i+j]/RES;
    }
    Normalised[10*i+6]=Triangles[10*i+6];
    Normalised[10*i+7]=Triangles[10*i+7];
    Normalised[10*i+8]=Triangles[10*i+8];
    Normalised[10*i+9]=Triangles[10*i+9];
  }
  return;
}

void InflateTriangles(double *Normalised, int TriangleCount, int Factor)
{
  int i,j=0;
  for(i=0;i<TriangleCount-1;i++){
    // Normalised points
    //printf("%d\n",i);
    for(j=0;j<6;j++){
      Normalised[10*i+j]=Normalised[10*i+j]*Factor;
    }
    Normalised[10*i+6]=Normalised[10*i+6];
    Normalised[10*i+7]=Normalised[10*i+7];
    Normalised[10*i+8]=Normalised[10*i+8];
    Normalised[10*i+9]=Normalised[10*i+9];
  }
  return;
}

int main (int argc, char *argv[]){
  int i,j,k, Evolved=0, Changed=0, TriangleCount, GenCount, Reverting=0, Divisions=20;
  int dimensions[2]={0, 0}, OutputRes, TriangStepping, FitThresholdAdjuster;
  FILE *input, *output;
  char Filename[100], TempString[300];
  double ***image, ***AuxMatrix, ***AuxMatrix2, init, PNMColor; 
  double *Triangles=NULL, *NormalisedTriangles=NULL;
  double ActualFit, FitStepping, FitThreshold, FitLimit, Magic;
  char Previous[150];
  char *Arg[12]={"-h","-file","-continue", "-revert", "-outputres", "-numtriangles", "-maxsteps", "-maxgenerations", "-verbose", "\n"};

  /*----------------------------------.
    | Processing command line arguments |
    `----------------------------------*/

  sprintf(Filename,"input.pnm");
  OutputRes=200;
  argc--;
  while(argc>=0){
    for(i=0;i<9+1;i++){
      if(strstr(argv[argc],Arg[i])!=NULL){
	switch (i){
	case 0:
	  puts("Use -file \"filename.xxx\" to process this file for triangularisation");
	  puts("Use -numtriangles N to set a number of triangles to process");
	  puts("Use -maxgenerations N to set a number of generations to iterate");
	  puts("Use -maxsteps N to set a maximum number of steps to process each generation");
	  puts("Use -verbose to output data to the screen");
	  puts("Use -continue \"filename.tri\" together with -file to continue the processing");
	  puts("Use -revert \"filename.tri\" to convert the triangle file to a jpg. Default resolution 300px");
	  puts("Use -outputres N to set the output resolution. Defaults to 200");
	  exit(1);
	  break;
	case 1:
	  sprintf(Filename,"%s",argv[argc+1]);
	  printf("Selected file: %s\n", argv[argc+1]);
	  break;
	  //{"-h","-file","-continue", "-revert", "-outputres", "-numtriangles", "-maxsteps", "-maxgenerations", "-verbose"}
	case 3:
	  sprintf(Filename,"%s",argv[argc+1]);
	  printf("Reverting file: %s\n", argv[argc+1]);
	  input=fopen(Filename,"r");
	  if(input==NULL){puts("Failure opening file"); exit(2);}
	  fscanf(input," %d",&TriangleCount);
	  NormalisedTriangles=(double*)malloc(TriangleCount*sizeof(double));
	  TriangleCount++;
	  for(i=0;i<10*TriangleCount;i++){
	    fscanf(input, " %lf",&NormalisedTriangles[i]);
	  }
	  puts("File parsed");
	  Reverting=1;
	  break;
	  
	case 4:
	  OutputRes=atoi(argv[argc+1]);
	  printf("Output resolution selected: %d\n",OutputRes);
	  
	  break;
	case 5:
	  MaxTriangles=atoi(argv[argc+1]);
	  printf("Maximum number of triangles selected: %d\n",MaxTriangles);
	  break;
	case 6:
	  MaximumSteps=atoi(argv[argc+1]);
	  printf("Maximum number of steps selected: %d\n",MaximumSteps);
	  break;
	case 7:
	  Generations=atoi(argv[argc+1]);
	  printf("Maximum number of generations selected: %d\n",Generations);
	  break;
	case 8:
	  VERBOSE=1;
	  break;
	}
      }
    }
    argc--;
  }
  puts("Allocating memory");
  
  dimensions[0]=RES;
  dimensions[1]=RES;

  Triangles=(double*)malloc(10*MaxTriangles*sizeof(double));
  if(!Reverting)NormalisedTriangles=(double*)malloc(10*MaxTriangles*sizeof(double));

  srand ( time(0) );
  image=(double***)malloc(4*sizeof(double**));
  image[0]=(double**)malloc(RES*sizeof(double*));
  image[1]=(double**)malloc(RES*sizeof(double*));
  image[2]=(double**)malloc(RES*sizeof(double*));
  image[3]=(double**)malloc(RES*sizeof(double*));
  for(i=0;i<RES;i++){
    image[0][i]=(double*)calloc(RES,sizeof(double));
    image[1][i]=(double*)calloc(RES,sizeof(double));
    image[2][i]=(double*)calloc(RES,sizeof(double));
    image[3][i]=(double*)calloc(RES,sizeof(double));
  }

  for(i=0;i<RES;i++){
    for(j=0;j<RES;j++){
      image[0][i][j]=BaseColor[0];
      image[1][i][j]=BaseColor[1];
      image[2][i][j]=BaseColor[2];
    }
  }

  AuxMatrix=(double***)malloc(4*sizeof(double**));
  AuxMatrix[0]=(double**)malloc(RES*sizeof(double*));
  AuxMatrix[1]=(double**)malloc(RES*sizeof(double*));
  AuxMatrix[2]=(double**)malloc(RES*sizeof(double*));
  AuxMatrix[3]=(double**)malloc(RES*sizeof(double*));
  for(i=0;i<RES;i++){
    AuxMatrix[0][i]=(double*)calloc(RES,sizeof(double));
    AuxMatrix[1][i]=(double*)calloc(RES,sizeof(double));
    AuxMatrix[2][i]=(double*)calloc(RES,sizeof(double));
    AuxMatrix[3][i]=(double*)calloc(RES,sizeof(double));
  }

  AuxMatrix2=(double***)malloc(4*sizeof(double**));
  AuxMatrix2[0]=(double**)malloc(OutputRes*sizeof(double*));
  AuxMatrix2[1]=(double**)malloc(OutputRes*sizeof(double*));
  AuxMatrix2[2]=(double**)malloc(OutputRes*sizeof(double*));
  AuxMatrix2[3]=(double**)malloc(OutputRes*sizeof(double*));
  for(i=0;i<OutputRes;i++){
    AuxMatrix2[0][i]=(double*)calloc(OutputRes,sizeof(double));
    AuxMatrix2[1][i]=(double*)calloc(OutputRes,sizeof(double));
    AuxMatrix2[2][i]=(double*)calloc(OutputRes,sizeof(double));
    AuxMatrix2[3][i]=(double*)calloc(OutputRes,sizeof(double));
  }

  if(Reverting){
    puts("Intro Reverting");
    InflateTriangles(NormalisedTriangles, TriangleCount, OutputRes);
    puts("Triangles inflated to resolution");
    GenerateMatrix(NormalisedTriangles, AuxMatrix2, TriangleCount, OutputRes);
    sprintf(Filename, "Reverted.ppm");
    output=fopen(Filename, "w");
    dimensions[0]=OutputRes;
    dimensions[1]=OutputRes;
    GenerarImatge(dimensions, AuxMatrix2, output,NULL);
    fclose(output);
    return 0;
  }

  sprintf(TempString,"convert -format pnm -compress none -resize %dx%d %s input.pnm", RES, RES, Filename);
  i=system(TempString);
  printf("System call: %s\n",TempString);
  printf("Result from system call: %d\n",i);
  if(i!=0){puts("There was an error processing your input image through ImageMagick");exit(1);}
  input=fopen("input.pnm","r");
  output=fopen("Sortida.ppm","w");
  fscanf(input, " %s", Filename); //get rid of P3;
  fscanf(input, " %lf ", &image[0][0][0]); //get rid of resolution
  fscanf(input, " %lf ", &image[0][0][0]); //get rid of resolution
  fscanf(input, " %lf ", &PNMColor); //get maxcolor
  
  printf("Base PNM color: %3.0lf\n",PNMColor);
  puts("Getting image");

  for(j=0;j<RES;j++){
    for(i=0;i<RES;i++){
      fscanf(input, " %lf ", &image[0][i][j]);
      image[0][i][j]=image[0][i][j]/PNMColor*255.0;
      fscanf(input, " %lf ", &image[1][i][j]);
      image[1][i][j]=image[1][i][j]/PNMColor*255.0;
      fscanf(input, " %lf ", &image[2][i][j]);
      image[2][i][j]=image[2][i][j]/PNMColor*255.0;
    }
  }
  BaseColor[0]=255;
  BaseColor[1]=255;
  BaseColor[2]=255;
  BaseColor[0]=0;
  BaseColor[1]=0;
  BaseColor[2]=0;
 
  AverageColor(RES, image);

  for(i=0;i<RES;i++){
    for(j=0;j<RES;j++){
      AuxMatrix[0][i][j]=BaseColor[0];
      AuxMatrix[1][i][j]=BaseColor[1];
      AuxMatrix[2][i][j]=BaseColor[2];
    }
  }
  
  for(j=0;j<MaxTriangles;j++){
    RandBlack(Triangles+10*j);
  }

  TriangleCount=0;
  GenerateMatrix(Triangles, AuxMatrix, TriangleCount, RES);
  init=MatrixDistance(RES, AuxMatrix, image);
  //  FitStepping=(init-SpeedLimit)/MaxTriangles*2.5;
  TriangStepping=MaxTriangles/Divisions;
  FitThreshold=init-FitStepping;
  FitLimit=SpeedLimit;
  FitThresholdAdjuster=Divisions;
  NumTriangles=TriangStepping;
//  NumTriangles=MaxTriangles;
  printf("Initial number of triangles: %d, maximal number of triangles %d\n\n",NumTriangles, MaxTriangles);
  printf("Initial fit %6.3lf, Fit Step %6.3lf\n\n",init, FitStepping);
  system("sleep 2");
  k=0;

  ActualFit=1e10;
  for(GenCount=0;GenCount<Generations;GenCount++){
    Evolved=0;
    GenerateMatrix(Triangles, AuxMatrix, TriangleCount, RES);
    ActualFit=MatrixDistance(RES, AuxMatrix, image);
    // Divide 1-0 in N intervals
    Magic=(ActualFit-SpeedLimit)/(init-SpeedLimit);
    Magic=Magic<0?Magic:sqrt(Magic);
    if(VERBOSE){printf("MAGIC: \t%lf, \tFT/NUM %lf\n",Magic, (double)(FitThresholdAdjuster)/Divisions);}
    if(Magic<((double)FitThresholdAdjuster)/Divisions){
      FitThresholdAdjuster--;
      if(FitThresholdAdjuster<0)FitThresholdAdjuster=0;
      NumTriangles+=TriangStepping;
      if(NumTriangles>MaxTriangles){NumTriangles=MaxTriangles;}else{
	if(VERBOSE){printf("IMPROVING: Adding triangles, reducing threshold\n");}}
    }
    FitThreshold=SpeedLimit+((double)FitThresholdAdjuster/Divisions)*((double)FitThresholdAdjuster/Divisions)*(init-SpeedLimit);
    if(VERBOSE){
      if(Previous[0]=='\0'){sprintf(Previous,"NOTHING");}
      printf("ITERATION %d:\n\n PREVIOUS: %s F1 %6.3lf \t Threshold: %6.3lf \t#Tr %d Max #Tr %d\n\n", GenCount, Previous, ActualFit, FitThreshold, TriangleCount, NumTriangles);}
    Previous[0]='\0';
    //Adding Triangles
    if(VERBOSE){printf("Generation: %10d\n",GenCount);}
    Changed=TriangleAdder(Triangles, &ActualFit, &TriangleCount, AuxMatrix, image, Changed);
    if(Changed){Evolved=1;sprintf(Previous, "%s ADDED", Previous);}
    if(VERBOSE){printf("Generation: %10d\n",GenCount);}
    
    if(((rand()%RANDOM==0)||(TriangleCount>=NumTriangles))&&(TriangleCount>0)){
      Changed=TriangleChanger(Triangles, &ActualFit, &TriangleCount, AuxMatrix, image, Changed);
      if(Changed==2){Evolved=1;sprintf(Previous, "CHANGED");}
      if(Changed==6){Evolved=1;sprintf(Previous, "JIGGED");}
      }
    if(VERBOSE){printf("Generation: %10d\n",GenCount);}
    if(((Changed)<=2||(RANDOM-1)||(TriangleCount>=NumTriangles))&&TriangleCount>0){
      if(rand()%(RandomSwapper)==0){
	Changed=TriangleSwapper(Triangles, &ActualFit, &TriangleCount, AuxMatrix, image);
	if(Changed){Evolved=1;sprintf(Previous, "%s SWAPPED", Previous);}}
      if(rand()%(RandomMover)==0){
	Changed=TriangleMover(Triangles, &ActualFit, &TriangleCount, AuxMatrix, image);
	if(Changed){Evolved=1;sprintf(Previous, "%s MOVED", Previous);}}
      if(rand()%(RandomPruner)==0){
	Changed=TrianglePruner(Triangles, &ActualFit, &TriangleCount, AuxMatrix, image);
      	if(Changed){sprintf(Previous, "%s PRUNED", Previous);}}
      if(VERBOSE){Evolved=1;printf("Generation: %10d\n",GenCount, GenCount);}
    }

    /* if(VERBOSE&&DEBUG){ */
/*       GenerateMatrix(Triangles, AuxMatrix, TriangleCount, RES); */
/*       init=MatrixDistance(RES, AuxMatrix, image); */
/*       printf("TESTING:\tF  %3.3lf\n",init); */
/*     } */
 
    if(GenCount%10==0&&Evolved&&TriangleCount>0){
      NormaliseTriangles(Triangles, NormalisedTriangles, TriangleCount);
      sprintf(Filename, "Generation%07d.tri", GenCount);
      output=fopen(Filename, "w");
      fprintf(output,"%d\n",TriangleCount);
      for(i=0;i<10*TriangleCount;i++){
	fprintf(output, "%7.7lf\n",NormalisedTriangles[i]);
      }
      fclose(output);
      InflateTriangles(NormalisedTriangles, TriangleCount, OutputRes);
      GenerateMatrix(NormalisedTriangles, AuxMatrix2, TriangleCount, OutputRes);
      sprintf(Filename, "Generation%07d.ppm", GenCount);
      output=fopen(Filename, "w");
      dimensions[0]=OutputRes;
      dimensions[1]=OutputRes;
      GenerarImatge(dimensions, AuxMatrix2, output,NULL);
      fclose(output);
      printf("%s written\n", Filename);
      
      /*  output=fopen("Triangles.tri", "w");
	  CharTriangles(Triangles, NumTriangles, BigText);
	  fprintf(output, "Generation %d \n %s \n\n",i,BigText);
	  for(j=0;j<10+80*NumTriangles;j++){
	  BigText[i]='a';
	  }
	  fclose(output);
	  printf("Triangles.tri written\n");*/
    }
    if(i%NumTriangles==0){Evolved=0;}
  }
  output=fopen("Final.ppm", "w");
  GenerarImatge(dimensions, AuxMatrix, output,NULL);
  return 0;
}
