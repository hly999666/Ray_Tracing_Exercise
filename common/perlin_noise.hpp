#ifndef PERLIN_NOISE_H
#define PERLIN_NOISE_H
#ifndef VEC3_H
#include "vec3.hpp"
#endif
#ifndef GENERAL_HELPER_H
#include  "general_helper.hpp"
#endif


static double* perlin_generate(){
   double* p=new double[256];
   for(int i=0;i<256;i++){
       p[i]=rand0_1();
   }
    return p;
};

void permute(int * p,int n){
      for(int i=n-1;i>0;i--){
          int target=int(rand0_1()*(i+1));
          int temp=p[i];
          p[i]=p[target];
          p[target]=temp;
      }
      return;
};

static int* perlin_genrate_perm(){
     int* p=new int[256];
     for(int i=0;i<256;i++)p[i]=i;
     permute(p,256);
     return p;
}

class perlin{
  public:
     static double* ranDouble;
     static int* perm_x;
     static int* perm_y;
     static int* perm_z;
     double noise(const vec3&p,  double scale=4.0)const{
          double u=p.x()-floor(p.x());
          double v=p.y()-floor(p.y());
          double w=p.z()-floor(p.z());
          int i=int(scale*p.x())&255;
          int j=int(scale*p.y())&255;
          int k=int(scale*p.z())&255;
          return ranDouble[perm_x[i]^perm_y[j]^perm_z[k]];
     };
};
double * perlin::ranDouble=perlin_generate();
int * perlin::perm_x=perlin_genrate_perm();
int * perlin::perm_y=perlin_genrate_perm();
int * perlin::perm_z=perlin_genrate_perm();
#endif