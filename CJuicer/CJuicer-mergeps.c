#include <stdio.h>
#include <stdlib.h>

#define LINEWIDTH 10000
#define STEPSIZE 100
void clean(char *str, int dim);

int main(int argc, char *argv[]){
// Argv contains the interesting functions' names, last element is the
// output file (i.e. you have files aaa.ps, bbb.ps and want to output
// to student1.pdf, you call the program with arguments aaa bbb
// student1 and press enter. You'll need ghostscript isntalled for it
// to work, though.
  int i,j, End=0;
  FILE **SetOfFiles, *result;
  char Filename[200];
  char *LongString;
  SetOfFiles=(FILE**)malloc((argc-1)*sizeof(FILE*));
  LongString=(char*)malloc((LINEWIDTH*sizeof(char)));
  if(SetOfFiles==NULL){puts("Failure to initialize memory. Strange.");exit(1);}
  printf("%d arguments given\n",argc);
  for(i=1;i<argc-1;i++){
    printf("Opening %s\n",argv[i]);
    sprintf(Filename, "%s", argv[i]);
    SetOfFiles[i]=fopen(Filename,"r");
    if(SetOfFiles[i]==NULL){
      printf("Failure opening %s\n Exitting to shell\n", argv[i]);
      exit(1);
    }
  }
  result=fopen("Unite.ps", "w");
  for(i=1;i<argc-1;i++){
    fprintf(result, "newpath %d 0 moveto\n", i*STEPSIZE);
    End=0;
    do{
      j=0;
      do{
	End=fscanf(SetOfFiles[i],"%c", &LongString[j]);
	if(LongString[j]=='\n'){break;}
	j++;
	//printf("%d\n",j);	
      }while(End!=EOF);    
      
      fprintf(result, "%s", LongString);
      clean(LongString,LINEWIDTH);
    }
    while(End!=EOF);
    fprintf(result, "closepath stroke\n");
  }
  fprintf(result, "showpage\n");
  fclose(result);
  sprintf(LongString,"gs -dBATCH -dNOPAUSE -sOutputFile=%s.pdf -sDEVICE=pdfwrite -c \"<< /PageSize [842 595] >> setpagedevice\" -f Unite.ps", argv[argc-1]);
  printf("System call: %s\n\n", LongString);
  system(LongString);
  return 0;
}

void clean(char *str, int dim){
  int i;
  for(i=0;i<dim;i++){
    str[i]='\0';
  }}
