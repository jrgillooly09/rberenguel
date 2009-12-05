//provaa

void Header(FILE *sortida, char *Comentari)
{
  fprintf(sortida,"P3\n");
  fprintf(sortida,"# %s\n",Comentari);
  fprintf(sortida,"%d %d\n",dimensions[0],dimensions[1]);
  fprintf(sortida,"255\n");
  return;
}

void OmplirMatriuFree(double *Parametres, int *dimensions,double *z, double *RGB, double ***Imatge, FILE *sortida, char *Comentari)
{
  /* This version starts in upper left corner, and ignores matrix
     contents. Expected values: upper left coordinate (Real and
     imaginary part, assumed), width and height (x,y,w,h)=Parametres.
     Dimensions is the pixel size, z is the point and RGB is the
     color. Imatge is not used, just for "almost compatibility" with
     the matrix version.  */
  
  int x,y;
  if((z[0]>Parametres[0]+Parametres[2])||z[0]<Parametres[0]){return;}
  if((z[1]>Parametres[1])||z[1]<Parametres[1]-Parametres[3]){return;}
  x=(int)floor((z[0]-Parametres[0])/Parametres[2]*dimensions[0]+0.1);
  y=(int)floor((Parametres[1]-z[1])/Parametres[3]*dimensions[1]+0.1);
  if((x>=dimensions[0])||(y>=dimensions[1])){return;}
  fprintf(sortida,"%3.0lf %3.0lf %3.0lf\n",RGB[0],RGB[1],RGB[2]);
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
      
    }
  }
  return;
}
