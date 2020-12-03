#include<random>
#include<chrono>
#include <cstdlib>
#ifndef GENERAL_HELPER_H
#define GENERAL_HELPER_H
#endif
#ifndef VEC3_H
  #include "vec3.hpp"
#endif

inline double random_double() {
 
    return (double)rand() / ( (double)RAND_MAX + 1.0);
}

inline double random_double(double min, double max) {
    
    return min + (max-min)*random_double();
}

inline int random_int(int min, int max) {
    return static_cast<int>(random_double(min, max+1.0));
}
 vec3 random_in_unit_circle(){
              vec3 p;
              do{
                    p=2.0*vec3(random_double(),random_double(),0)-vec3(1,1,0);
              }while(dot(p,p)>=1.0);
            return p; 
}
vec3 random_in_unit_sphere(){
 
    vec3 ans(random_double(),random_double(),random_double());
     ans=ans*2.0-vec3(1.0,1.0,1.0);
     while(dot(ans,ans)>=1.0){

       ans=vec3(random_double(),random_double(),random_double());
        ans=ans*2.0-vec3(1.0,1.0,1.0);
     }
     return ans;
 };
const unsigned long long  _seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
class random_tool{
   public:
    unsigned  long long  seed{_seed};
   std::mt19937_64 mt_r{_seed};
   std::uniform_real_distribution<double>dist{0.0,1.0};
  random_tool(){ 
    const long long  seed_n = std::chrono::high_resolution_clock::now().time_since_epoch().count();
            seed=seed_n;
            mt_r.seed(seed_n);
    };
   double rand(){
          return dist(mt_r);

    };
 vec3 random_in_unit_sphere(){
 
    vec3 ans(dist(mt_r),dist(mt_r),dist(mt_r));
     ans=ans*2.0-vec3(1.0,1.0,1.0);
     while(dot(ans,ans)>=1.0){

       ans=vec3(dist(mt_r),dist(mt_r),dist(mt_r));
        ans=ans*2.0-vec3(1.0,1.0,1.0);
     }
     return ans;
 };
};

