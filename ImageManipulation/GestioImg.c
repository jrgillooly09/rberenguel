//provaa

void OmplirMatriu(double *Parametres, int *dimensions,double *z, int *RGB, double ***Imatge)
{
  /*
    Els par�metres s�n: Coordenada superior esquerra (Re i Im), amplada i al�ada.
    Dimensions s�n nombre de pixels d'amplada i al�ada respectivament
    RGB cont� el color associat al punt z, 
    Imatge �s int[4][x][y]
  */
	
  int x,y;
  if((z[0]>Parametres[0]+Parametres[2])||z[0]<Parametres[0]){return;}
  if((z[1]>Parametres[1])||z[1]<Parametres[1]-Parametres[3]){return;}
  x=(int)floor((z[0]-Parametres[0])/Parametres[2]*dimensions[0]+0.1);
  y=(int)floor((Parametres[1]-z[1])/Parametres[3]*dimensions[1]+0.1);
  /*	printf("Valors%7.5lf %7.5lf\n",(z[0]-Parametres[0])/Parametres[2]*(double)dimensions[0],(Parametres[1]-z[1])/Parametres[3]*(double)dimensions[1]);
	printf("Puntsx=%d y=%d, z=(%7.6lf, %7.6lf), dim=%d %d\n",x,y,z[0],z[1], dimensions[0],dimensions[1]);*/
  //if(Imatge[3][x][y]>0){printf("Error!%7.6lf\n",Imatge[3][x][y]);exit(1);}
  if((x>=dimensions[0])||(y>=dimensions[1])){return;}
  Imatge[3][x][y]++;
  Imatge[0][x][y]=Imatge[0][x][y]+(RGB[0]-Imatge[0][x][y])/(Imatge[3][x][y]);
  Imatge[1][x][y]=Imatge[1][x][y]+(RGB[1]-Imatge[1][x][y])/(Imatge[3][x][y]);
  Imatge[2][x][y]=Imatge[2][x][y]+(RGB[2]-Imatge[2][x][y])/(Imatge[3][x][y]);
	
  return;
}

void GenerarImatge(int *dimensions, double ***Imatge, FILE *sortida, char *Comentari)
{
  /* 
     Escriu la imatge a un fitxer PPM, afegint el comentari adequat
  */
  int i,j;
  fprintf(sortida,"P3\n");
  fprintf(sortida,"# %s\n",Comentari);
  fprintf(sortida,"%d %d\n",dimensions[0],dimensions[1]);
  fprintf(sortida,"255\n");
  for(j=0;j<dimensions[1];j++){
    for(i=0;i<dimensions[0];i++){
      fprintf(sortida,"%3.0lf %3.0lf %3.0lf\n",Imatge[0][i][j],Imatge[1][i][j],Imatge[2][i][j]);
    }
  }
  return;
}


void Header(int *dimensions, FILE *sortida, char *Comentari)
{
  fprintf(sortida,"P3\n");
  fprintf(sortida,"# %s\n",Comentari);
  fprintf(sortida,"%d %d\n",dimensions[0],dimensions[1]);
  fprintf(sortida,"255\n");
  return;
}

void OmplirMatriuFree(double *Parametres, int *dimensions,double *z, double *RGB, double ***Imatge, FILE *sortida)
{
  /* This version starts in upper left corner, and ignores matrix
     contents. Expected values: upper left coordinate (Real and
     imaginary part, assumed), width and height (x,y,w,h)=Parametres.
     Dimensions is the pixel size, z is the point and RGB is the
     color. Imatge is not used, just for "almost compatibility" with
     the matrix version.  */
  
  /* Aquesta versi� simplement escriu comen�ant per la cantonada
     superior esquerra i ignora els continguts de la matriu. Afegeixo
     un par�metre a la versi� anterior, que �s el fitxer. A m�s passo
     l'RGB a double i la matriu imatge tamb�. Els par�metres s�n:
     Coordenada superior esquerra (Re i Im), amplada i al�ada.
     Dimensions s�n nombre de pixels d'amplada i al�ada respectivament
     RGB cont� el color associat al punt z, Imatge �s int[4][x][y]  */

  fprintf(sortida,"%3.0lf %3.0lf %3.0lf\n",RGB[0],RGB[1],RGB[2]);
  return;
}
