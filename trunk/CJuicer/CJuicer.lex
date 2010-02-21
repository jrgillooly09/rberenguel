L    [a-zA-Z]
D    [0-9]
/* Here come some definitions standing for "Letter" and "Digit" */
%{
  #include <string.h>
  #define STACKSIZE 200
  #define FUNCT 200
  #define FOR 1
  #define IF 2
  #define DO 3
  #define WHILE 4
  #define FORMOVE 5
  #define WHILEMOVE 7
  #define IFMOVE 9
  #define FUNMOVE 15
  #define VERTMOVE 5
  #define FORNUM 1
  #define WHILENUM 3
  #define IFNUM 5
  #define FORNUMOUT 2
  #define WHILENUMOUT 4
  #define IFNUMOUT 6
  #define FUNNUM 7

  /* A whole bunch oh global variables (to the output lexer) and
     defines for the graphic-number output system. The goal of all
     global variables is to be able to parse code, knowing where in a
     loop we are, for instance. We also build a fixed size stack to
     keep track of function call order. The stack is not extremely
     big, but can be changed if necessary (I needed to once, as the
     number was lower)  */


  FILE *auxiliar, *auxiliarNUM;
  char Function[FUNCT], Filename[FUNCT];
  int key=0, countkey=0, include=0, def=0, par=0;
  int RecentKey=0;
  int InsideFor=0, InsideIf=0, InsideWhile=0, InsideDo=0;
  int Last=0;
  int stack[STACKSIZE], stackcount=0;
  void clean(char *str, int dim);
  void SetInside(int type, int flag);
  void GetIn(int type); 
  void GetOut(int type);
  void RemoveSpace(char *str, int dim);
%}
/*
"sizeof"+" "{1,5} {}
"sizeof"+" "*+"("     {par++;}
 */

/* Now comes the lexer part. To each string incoming string, we want
   the program to do the output rule. For instance, if we find
   "extern" we want to set def=0, to avoid the assumption we are
   defining a function.  */

/* This never-ending set of rules is somehow the heart of the program,
   as it enables us to keep track of where we are. Scattered through
   some special operands (like ;) we have logging code.  */

/* The crux of the matter is the keyword matcher, the drawer/numberer.
   Every time we find a "work" operator, like for, if, do, or a
   function call we "do something else", i.e. call one of the special
   purpose functions.  */

/* At the end of the main chunk of keyword detectors comes the
   "trickiest" code: detecting whether we are
   defining/calling/entering a function. It is somewhat hard to
   follow, but it is just based on whether we are inside a {} open
   block, or in "define mode" (def=1)  */
%%
"extern"     {def=0;}
"include"     {include=1;}
"fprintf"+" "{1,5} {}
"fprintf"+" "*+"("     {par++;}
"fscanf"+" "{1,5} {}
"fscanf"+" "*+"("     {par++;}
"printf"+" "{1,5} {}
"printf"+" "*+"("     {par++;}
"puts"+" "{1,5} {}
"puts"+" "*+"("     {par++;}
"scanf"+" "{1,5} {}
"scanf"+" "*+"("     {par++;}
"^int"         {def=key?1:0;}
"^int*"        {def=key?1:0;}
"^double"      {def=key?1:0;}
"^double*"     {def=key?1:0;}
" "*+"double"      {def=key?1:0;}
" "*+"double*"     {def=key?1:0;}
"("+" "*+"double"            {par++;def=0;}
"("+" "*+"double"+" "*+"*"   {par++;def=0;}
"FILE"        {def=key?1:0;}
"FILE*"       {def=key?1:0;}
"void"        {def=0;}
"char*"       {def=key?1:0;}
";"           {def=0;  
  fprintf(auxiliar, "%% Flags: inc %d def %d par %d for %d RecKey %d\n",include, def, par, InsideFor, RecentKey);
  if(!par&&(InsideFor||InsideIf)&&!RecentKey){GetOut(Last);}RecentKey=0;}
"("           {par++;}
")"           {par--;}
"<"           {}
">"           {include=0;}
"for"+" "{1,5} {GetIn(FOR);}
"for"+" "*+"(" {par++;GetIn(FOR);}
"if"+" "{1,5} {GetIn(IF);}
"if"+" "*+"("  {par++;GetIn(IF);}
"do"          {GetIn(WHILE);}
"do("         {GetIn(WHILE);}
"while"+" "{1,5}       {if(!InsideDo)GetIn(WHILE);}
"while("      {par++;if(!InsideDo)GetIn(WHILE);}
{L}{1,100}+{D}*" "{0,5}"("  {
  if(!include&&!def){//&&!par
    strncpy(Function, yytext, yyleng-1);
    RemoveSpace(Function, FUNCT);
    if(!key&&!par){
      //fprintf(auxiliar, "closepath stroke showpage\n");
      sprintf(Filename, "%s.ps", Function);
      auxiliar=fopen(Filename,"w");
      sprintf(Filename, "%s.num", Function);
      auxiliarNUM=fopen(Filename,"w");
      fprintf(auxiliar, "%% Function: %s\n\n 30 500 rmoveto\n", Function);
      fprintf(auxiliar,"/Times-Roman findfont\n 20 scalefont\n setfont\n");
      fprintf(auxiliar,"(%s) show\n -%d -%d rmoveto\n",Function,50, 10);
      fprintf(auxiliar,"/Times-Roman findfont\n 8 scalefont\n setfont\n");
      fprintf(auxiliarNUM,"* \n");
    }
    else{
      if(key){
      fprintf(auxiliar, "%% Crida a funcio %s\n",Function);
      fprintf(auxiliar, "%d -%d rlineto 0 -%d rlineto\n", FUNMOVE, 0, VERTMOVE/2);
      fprintf(auxiliar, "gsave\n  3 -1 rmoveto (%s) show\n grestore\n", Function);
      fprintf(auxiliar, "0 -%d rlineto \n -%d 0 rlineto\n",VERTMOVE/2, FUNMOVE);}
      fprintf(auxiliarNUM, "%d ", FUNNUM);
    }
    }else{
    fprintf(auxiliar, "%% Flags: inc %d def %d par %d Crida a funcio %s\n",include, def, par, Function);
  }
	clean(Function, FUNCT);par++;}

"{"           {key++;countkey++;RecentKey=1;}
"}"           {RecentKey=0;key--;if(key){GetOut(Last);}else{
      fprintf(auxiliar, "0 0 rmoveto \n");fprintf(auxiliarNUM, "\n");}}
%%

/* The following definitions are just the auxiliary functions needed,
   and can be understood without too much hassle  */

int main(void)
{
  auxiliar=fopen("test.dat","w");
  yylex();
  return 0;

}
void clean(char *str, int dim){
  int i;
  for(i=0;i<dim;i++){
    str[i]='\0';
  }}

void RemoveSpace(char *str, int dim){
  int i;
  for(i=0;i<dim;i++){
    if(str[i]==' '){str[i]='\0';}
  }}

void GetIn(int type){
  switch (type){
  case FOR :
    fprintf(auxiliar, "%d -%d rlineto %% Into for\n", FORMOVE, VERTMOVE);
    fprintf(auxiliarNUM, "%d", FORNUM);
    SetInside(FOR, 1);
    break;
  case IF:
    fprintf(auxiliar, "%d -%d rlineto  %% Into if\n", IFMOVE, VERTMOVE);
    fprintf(auxiliarNUM, "%d", IFNUM);
    SetInside(IF, 1);
    break;
  case WHILE:
    fprintf(auxiliar, "%d -%d rlineto  %% Into while\n", WHILEMOVE, VERTMOVE);
    fprintf(auxiliarNUM, "%d", WHILENUM);
    SetInside(WHILE, 1);
    break;
  default :
    break;
  }  
}

 void GetOut(int type){
  switch (type){
  case FOR :
    fprintf(auxiliar, "-%d  -%d rlineto %% Exit for\n", FORMOVE, VERTMOVE);
    fprintf(auxiliarNUM, "%d", FORNUMOUT);
    SetInside(FOR,0);
    break;
  case IF:
    fprintf(auxiliar, "-%d -%d rlineto %% Exit if\n", IFMOVE, VERTMOVE);
    fprintf(auxiliarNUM, "%d", IFNUMOUT);
    SetInside(IF,0);
    break;
 case WHILE:
    fprintf(auxiliar, "-%d -%d rlineto  %% Exit while\n", WHILEMOVE, VERTMOVE);
    fprintf(auxiliarNUM, "%d", WHILENUMOUT);
    SetInside(WHILE, 0);
    break;
  default :
    break;
  }  
}

 void SetLast(int type)
 {
   stackcount++;
   if(stackcount==STACKSIZE){
     printf("Over the size of the stack!\n");
     printf("Num keys: %d, stackcount: %d\n\n", countkey, stackcount);
     exit(1);}
   stack[stackcount]=type;
   Last=type;
 }

 void PopLast()
 {
   stack[stackcount]=0;
   stackcount--;
   Last=stack[stackcount];
 }
   
 void SetInside(int type, int flag){
  switch (type){
   case FOR :
    InsideFor=flag;
    if(flag){SetLast(FOR);}else{PopLast();}
    break;
   case IF:
    InsideIf=flag;
    if(flag){SetLast(IF);}else{PopLast();}
    break;
  case WHILE:
    InsideWhile=flag;
    if(flag){SetLast(WHILE);}else{PopLast();}
    break;
   default :
    break;
   } 
}
