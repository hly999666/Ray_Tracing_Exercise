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
inline double trilinear_interp(double c[2][2][2],double u,double v,double w){
    double sum=0.0;
    for(int i=0;i<2;i++){
      for(int j=0;j<2;j++){
        for(int k=0;k<2;k++){
            double w_x=(i*u+(1-i)*(1-u));
            double w_y=(j*v+(1-j)*(1-v));
            double w_z=(k*w+(1-k)*(1-w));
            sum+=w_x*w_y*w_z*c[i][j][k];
    }
    }
    }

    return sum;
}
class perlin{
  public:
     static double* ranDouble;
     static int* perm_x;
     static int* perm_y;
     static int* perm_z;
     double noise(const vec3&_p,  double freq=4.0)const{
           vec3 p=_p;p*=freq;
          double u=p.x()-floor(p.x());
          double v=p.y()-floor(p.y());
          double w=p.z()-floor(p.z());
          //hermite cubic interpolation
          u=u*u*(3-2*u);
          v=v*v*(3-2*v);
          w=w*w*(3-2*w);
          //use & as mark
    /*       int i=int(scale*p.x())&255;
          int j=int(scale*p.y())&255;
          int k=int(scale*p.z())&255; */
            
            int i=floor(p.x());
             int j=floor(p.y());
              int k=floor(p.z());
         double c[2][2][2];
            for(int dx=0;dx<2;dx++){
           for(int dy=0;dy<2;dy++){
            for(int dz=0;dz<2;dz++){
              c[dx][dy][dz]= ranDouble[perm_x[(i+dx)&255]^perm_y[(j+dy)&255]^perm_z[(k+dz)&255]];

        }}}
          return trilinear_interp(c,u,v,w);
     };
};
double * perlin::ranDouble=perlin_generate();
int * perlin::perm_x=perlin_genrate_perm();
int * perlin::perm_y=perlin_genrate_perm();
int * perlin::perm_z=perlin_genrate_perm();
#endif