/*
  Tarea #3 - Física Computacional
  Andrea Rozo Méndez
  201015972
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <gsl/gsl_linalg.h>

double *load_data(char *filein, int *n_filas, int columna);
double *transpose_data(double *datos, int n_filas, int columna);

int main (int argc, char **argv)
{
  char archivo[256];
  strcpy(archivo,argv[1]);
  double *tiempo, *posicion;
  int n_filas = 0;
  int n_columnas = 2;
  int t = 0;
  int p = 1;
  int i,x = 0;
  
  tiempo = load_data(archivo, &n_filas, t);
  posicion = load_data(archivo, &n_filas, p);

  gsl_matrix *G = gsl_matrix_calloc(n_filas,3);
  gsl_vector *d = gsl_vector_calloc(n_filas);
  gsl_matrix *G_t = gsl_matrix_calloc(3,n_filas);

  for (i=0;i<n_filas;i++)
  {
	gsl_matrix_set(G,i,0,1);
	gsl_matrix_set(G,i,1,tiempo[i]);
	gsl_matrix_set(G,i,2,0.5*tiempo[i]*tiempo[i]);

	gsl_vector_set(d,i,posicion[i]);

	gsl_matrix_set(G,0,i,1);
	gsl_matrix_set(G,1,i,tiempo[i]);
	gsl_matrix_set(G,2,i,0.5*tiempo[i]*tiempo[i]);
  }  
  
  //t_data = transpose_data(data, n_filas, n_columnas);
  
}
  
  double *load_data(char *filein, int *n_filas, int columna)
  {
    double *datos;
    FILE *in;
    int c;
    int nf = 0;
    int nc = 2;
    double df,dc;
    int i=0,j=0;
    in = fopen(filein,"r");
    if (!in)
      {
	    printf("problems opening the file %s\n", filein);
            exit(1);
      }
    
    do
      {
	        c = fgetc(in);
	        if(c=='\n')
	         {
	           nf++;
             }
      } while(c!=EOF);
    
    rewind(in);
    *n_filas = nf;
    datos = malloc(nf*sizeof(double));
    
    do
    {
          fscanf(in,"%f %f\n",&df,&dc);
	  if (columna == 0)
	  {         
		datos[i] = df;
	  }
	  else if (columna == 1)
	  {
		datos[i] = dc;
	  }
          i++;
    } while(i<nf);
   
    return datos;
  }
  
  double *transpose_data(double *datos, int n_filas, int n_columnas)
  {
        double *datos_t;
        int i,j,x=0;
        datos_t = malloc(n_filas*n_columnas*sizeof(double));
        
        for (i=0;i<n_filas;i++)
        {
            for (j=0;j<n_columnas;j++)
            {
                datos_t[i+(n_filas*j)] = datos[x];
                x++;
            }
        }
        
        return datos_t;
  }

