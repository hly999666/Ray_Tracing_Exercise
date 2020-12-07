#include<random>
#include<chrono>
#include<vector>
#include <cstdlib>
#ifndef GENERAL_HELPER_H
#define GENERAL_HELPER_H
#endif
#ifndef VEC3_H
  #include "vec3.hpp"
#endif

using std::sqrt;
class random_tool;
random_tool* now_rt;
const double inf=std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

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

inline double random_double() {
 
    //return (double)rand() / ( (double)RAND_MAX + 1.0);
    return now_rt->rand();
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


 void write_color( std::vector<vec3>& framebuffer,int pos,color& pixel_color, int samples_per_pixel) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Replace NaN components with zero. See explanation in Ray Tracing: The Rest of Your Life.
    if (r != r) r = 0.0;
    if (g != g) g = 0.0;
    if (b != b) b = 0.0;

    // Divide the color by the number of samples and gamma-correct for gamma=2.0.
    auto scale = 1.0 / samples_per_pixel;
    r = sqrt(scale * r);
    g = sqrt(scale * g);
    b = sqrt(scale * b);

  framebuffer[pos]=color(r,g,b);

  
};


inline double clamp(double x, double min, double max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
    };
 
 std::vector<unsigned char> convertFrameBuffer(const std::vector<vec3>& fb){
          std::vector<unsigned char>output;
          int size=fb.size();
          output.resize(size*3);
          int count=0;
          for(const vec3& c:fb){
            output[count++]=static_cast<unsigned char>(256 * clamp(c.b(), 0.0, 0.999));
             output[count++]=static_cast<unsigned char>(256 * clamp(c.g(), 0.0, 0.999));
              output[count++]=static_cast<unsigned char>(256 * clamp(c.r(), 0.0, 0.999));
          }
            return output;
    };