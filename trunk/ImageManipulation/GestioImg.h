void OmplirMatriu(double *Parametres, int *dimensions,double *z, int *RGB, double ***Imatge);

void GenerarImatge(int *dimensions, double ***Imatge, FILE *sortida, char *Comentari);

void OmplirMatriuFree(double *Parametres, int *dimensions,double *z, double *RGB, double ***Imatge, FILE *sortida);

void Header(int *dimensions, FILE *sortida, char *Comentari);
