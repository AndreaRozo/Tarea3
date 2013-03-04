/*
  Tarea #3 - Física Computacional
  Andrea Rozo Méndez
  201015972
  Parte 2
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_eigen.h>

double *load_data(char *filein, int *n_filas, int columna);
double mean(double *vector, int n_filas);
double covariance(double *vector1, double *vector2, double media1, double media2, int n_filas);

int main (int argc, char **argv)
{

  if(argc != 2)
  {
	printf("Faltan parámetros para correr el programa\n");
	exit(1);
  }

  char archivo[256];
  strcpy(archivo,argv[1]);
  double *col1,*col2,*col3;
  int n_filas = 0;
  int n_columnas = 3;
  double mean1,mean2,mean3,cov11,cov12,cov13,cov22,cov23,cov33;
  int i,j;

  //Se cargan las columnas del archvo por separado
  col1 = load_data(archivo, &n_filas, 0);
  col2 = load_data(archivo, &n_filas, 1);
  col3 = load_data(archivo, &n_filas, 2);

  //Se calcula la media de cada columna
  mean1 = mean(col1,n_filas);
  mean2 = mean(col2,n_filas);
  mean3 = mean(col3,n_filas);

  //Se calcula la covarianza entre cada par de columnas y se crea la matriz de covarianzas
  cov11 = covariance(col1,col1,mean1,mean1,n_filas);
  cov12 = covariance(col1,col2,mean1,mean2,n_filas);
  cov13 = covariance(col1,col3,mean1,mean3,n_filas);
  cov22 = covariance(col2,col2,mean2,mean2,n_filas);
  cov23 = covariance(col2,col3,mean2,mean3,n_filas);
  cov33 = covariance(col3,col3,mean3,mean3,n_filas);

  gsl_matrix *Cov = gsl_matrix_calloc(n_columnas,n_columnas);
  gsl_matrix_set(Cov,0,0,cov11);
  gsl_matrix_set(Cov,0,1,cov12);
  gsl_matrix_set(Cov,0,2,cov13);
  gsl_matrix_set(Cov,1,0,cov12);
  gsl_matrix_set(Cov,1,1,cov22);
  gsl_matrix_set(Cov,1,2,cov23);
  gsl_matrix_set(Cov,2,0,cov13);
  gsl_matrix_set(Cov,2,1,cov23);
  gsl_matrix_set(Cov,2,2,cov33);

  //Cálculo de los vectores y valores propios, ya se organizan en orden descendiente, los vectores propios ya se encuentras normalizados a magnitud unitaria y son mutuamente ortogonales
  gsl_vector *eval = gsl_vector_alloc (n_columnas);
  gsl_matrix *evec = gsl_matrix_alloc (n_columnas, n_columnas);
  gsl_eigen_symmv_workspace * w = gsl_eigen_symmv_alloc (n_columnas);
  gsl_eigen_symmv (Cov, eval, evec, w);
  gsl_eigen_symmv_free (w);
  gsl_eigen_symmv_sort (eval,evec,GSL_EIGEN_SORT_VAL_DESC);
  
  //Generación del archivo de salida
  FILE *out;
  out = fopen("autovectores_3D_data.dat","w");
  gsl_vector *evec_1 = gsl_vector_calloc(n_columnas);
  gsl_matrix_get_col(evec_1,evec,0);
  gsl_vector *evec_2 = gsl_vector_calloc(n_columnas);
  gsl_matrix_get_col(evec_2,evec,1);
  gsl_vector *evec_3 = gsl_vector_calloc(n_columnas);
  gsl_matrix_get_col(evec_3,evec,2);
  fprintf(out, "%f\n", gsl_vector_get(eval,0));
  fprintf(out, "%f %f %f\n", gsl_vector_get(evec_1,0),gsl_vector_get(evec_1,1),gsl_vector_get(evec_1,2));
  fprintf(out, "%f\n", gsl_vector_get(eval,1));
  fprintf(out, "%f %f %f\n", gsl_vector_get(evec_2,0),gsl_vector_get(evec_2,1),gsl_vector_get(evec_2,2));
  fprintf(out, "%f\n", gsl_vector_get(eval,2));
  fprintf(out, "%f %f %f\n", gsl_vector_get(evec_3,0),gsl_vector_get(evec_3,1),gsl_vector_get(evec_3,2));
  fclose(out);

  return 0;
}
  

  double *load_data(char *filein, int *n_filas, int columna)
  {
    double *datos;
    FILE *in;
    int c;
    int nf = 0;
    double d1,d2,d3;
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
          fscanf(in,"%lf %lf %lf\n",&d1,&d2,&d3);
	  if (columna == 0)
	  {         
		datos[i] = d1;
	  }
	  else if (columna == 1)
	  {
		datos[i] = d2;
	  }
	  else if (columna == 2)
	  {
		datos[i] = d3;
	  }
          i++;
    } while(i<nf);

    fclose(in);
   
    return datos;
  }

  double mean(double *vector, int n_filas)
  {
	double suma = 0,media;
	int i = 0;
	for (i = 0; i < n_filas; i++)
	{
		suma += vector[i];
	}
	media = suma/(n_filas-1);
	return media;
  }

  double covariance(double *vector1, double *vector2, double media1, double media2, int n_filas)
  {
	double suma = 0,cov;
	int i = 0;
	for (i = 0; i < n_filas; i++)
	{
		suma += ((vector1[i]-media1)*(vector2[i]*media2));
	}
	cov = suma/(n_filas-1);
	return cov;
  }


