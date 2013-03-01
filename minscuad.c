/*
  Tarea #3 - Física Computacional
  Andrea Rozo Méndez
  201015972
  Parte 1
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <gsl/gsl_linalg.h>

double *load_data(char *filein, int *n_filas, int columna);

int main (int argc, char **argv)
{
  if(argc != 2)
  {
	printf("Faltan parámetros para correr el programa\n");
	exit(1);
  }

  char archivo[256];
  strcpy(archivo,argv[1]);
  double *tiempo, *posicion;
  int n_filas = 0;
  int n_columnas = 2;
  int nc = 3;
  int t = 0;
  int p = 1;
  int i,j,a;
  double x,y,z;
    
  tiempo = load_data(archivo, &n_filas, t);
  posicion = load_data(archivo, &n_filas, p);

  //Creación de las matrices y vectores empleados en la regresión
  gsl_matrix *G = gsl_matrix_calloc(n_filas,nc);
  gsl_vector *d = gsl_vector_calloc(n_filas);
  gsl_matrix *G_T = gsl_matrix_calloc(nc,n_filas);

  for (i=0;i<n_filas;i++)
  {
	gsl_matrix_set(G,i,0,1);
	gsl_matrix_set(G,i,1,tiempo[i]);
	gsl_matrix_set(G,i,2,0.5*tiempo[i]*tiempo[i]);
	
	gsl_vector_set(d,i,posicion[i]);

	gsl_matrix_set(G_T,0,i,1);
	gsl_matrix_set(G_T,1,i,tiempo[i]);
	gsl_matrix_set(G_T,2,i,0.5*tiempo[i]*tiempo[i]);
  }
  
  //Cálculo del producto de G_T*G
  gsl_matrix *prod = gsl_matrix_calloc(nc,nc);
  for (i=0;i<nc;i++)
  {
	for (j=0;j<nc;j++)
	{
		z = 0;
		for (a=0;a<n_filas;a++)
		{
			x = gsl_matrix_get(G_T,i,a);
			y = gsl_matrix_get(G,a,j);
			z += x*y;
		}
		gsl_matrix_set(prod,i,j,z);
	}
  }

  //Cálculo de la inversa de (G_T*G)
  gsl_permutation *perm = gsl_permutation_alloc(nc);
  gsl_matrix *invProd = gsl_matrix_calloc(nc,nc);
  int signo;

  gsl_linalg_LU_decomp(prod,perm,&signo);
  gsl_linalg_LU_invert(prod,perm,invProd);

  //Cálculo del producto (G_T*G)-¹*G_T
  gsl_matrix *prod2 = gsl_matrix_calloc(nc,n_filas);
  for (i=0;i<nc;i++)
  {
	for (j=0;j<n_filas;j++)
	{
		z = 0;
		for (a=0;a<nc;a++)
		{
			x = gsl_matrix_get(invProd,i,a);
			y = gsl_matrix_get(G_T,a,j);
			z += x*y;
		}
		gsl_matrix_set(prod2,i,j,z);
	}
  }

  //Cálculo del vector de parámetros m = (G_T*G)-¹*G_T*d
  gsl_vector *m = gsl_vector_calloc(nc);
  for (i=0;i<nc;i++)
  {
	z = 0;
	for (j=0;j<n_filas;j++)
	{
		x = gsl_matrix_get(prod2,i,j);
		y = gsl_vector_get(d,j);
		z += x*y;
	}
	
	gsl_vector_set(m,i,z);
  }

  //Generación del archivo de salida
  FILE *out;
  out = fopen("parametros_movimiento.dat","w");
  fprintf(out, "%lf %lf %lf\n", gsl_vector_get(m,0), gsl_vector_get(m,1), gsl_vector_get(m,2));
  fclose(out);

  return 0;
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
          fscanf(in,"%lf %lf\n",&df,&dc);
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

    fclose(in);
   
    return datos;
  }


