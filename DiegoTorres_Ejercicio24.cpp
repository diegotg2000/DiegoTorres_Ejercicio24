#include <fstream>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <cmath>
#include <limits>
#include <math.h> 
#include <cstdlib>
#include <ctime>
#include <time.h>
float inf = -logf(0.0); 
using namespace std; 

float *read_file(string filename, int *n_points);
float *model(float*x, int n_points, float *c, int poly_degree);
float loglikelihood(float *x_obs, float *y_obs, int n_points, float *c, int poly_degree);
float logprior(float *c, int poly_degree);
void MCMC_polynomial(float *x_obs, float *y_obs, int n_points, int n_steps, int poly_degree);

int main(){
  float *x=NULL;
  float *y=NULL;
  int n_x=0;
  int n_y=0;

  x = read_file("valores_x.txt", &n_x);
  y = read_file("valores_y.txt", &n_y);

  srand48(time(0));  
  MCMC_polynomial(x, y, n_x, 1000000, 3);
  
  return 0;
}

float *read_file(string filename, int *n_points){
  int n_lines=0;
  ifstream infile; 
  string line;
  int i;
  float *array;

  /*cuenta lineas*/
  infile.open(filename); 
  getline(infile, line);
  while(infile){
    n_lines++;
    getline(infile, line);
  }
  infile.close();
  *n_points = n_lines;

  /*reserva la memoria necesaria*/
  array = new float[n_lines];

  /*carga los valores*/
  i=0;
  infile.open(filename); 
  getline(infile, line);  
  while(infile){
    array[i] = atof(line.c_str());
    i++;
    getline(infile, line);
  }
  infile.close();

  return array;
}

float logprior(float *c, int poly_degree){
    return 0;
}
float *model(float*x, int n_points, float *c, int poly_degree){
        float *result=NULL;
        result=new float[n_points];
        for(int i=0;i<n_points;i++){
            float punto=x[i];
            float suma=0;
            for(int n=0;n<poly_degree+1;n++){
                suma+=c[n]*pow(punto,n);
            }
            result[i]=suma;
        }
        return result;
}
float loglikelihood(float *x_obs, float *y_obs, int n_points, float *c, int poly_degree){
    float *mis_yes=NULL;
    mis_yes= new float[n_points];
    float suma;
    mis_yes=model(x_obs,n_points,c,poly_degree);
    for(int i=0;i<n_points;i++){
        suma+=(mis_yes[i]-y_obs[i])*(mis_yes[i]-y_obs[i]);        
    }
    return -0.5*suma;
}
void MCMC_polynomial(float *x_obs, float *y_obs, int n_points, int n_steps, int poly_degree){
    float antes,despues;
    float *c=new float[poly_degree+1];

    for(int m=0;m<(poly_degree+1);m++){
        c[m]=0;
    }
    for(int i=0;i<n_steps;i++){
        float *s=new float[poly_degree+1];
        for(int j=0;j<(poly_degree+1);j++){
            s[j]=c[j]+2*drand48()-1;
        }
        antes=loglikelihood(x_obs,y_obs,n_points, c,poly_degree);
        despues=loglikelihood(x_obs,y_obs,n_points, s,poly_degree);
        float gamma=exp(despues)/exp(antes);
        float r=drand48();
        if(gamma>r){
        for(int k=0;k<(poly_degree+1);k++){
            c[k]=s[k];
        }
        }
        cout<<c[0]<<' '<<c[1]<<' '<<c[2]<<' '<<c[3]<<endl;
    }  
}