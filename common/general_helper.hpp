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

class random_tool{
   public:
    unsigned  long long  seed;
   std::mt19937_64 mt_r;
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

vec3 convertColor(const color& pixel_color, int samples_per_pixel=1,double gamma=1.0){
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Replace NaN components with zero. See explanation in Ray Tracing: The Rest of Your Life.
    if (r != r) r = 0.0;
    if (g != g) g = 0.0;
    if (b != b) b = 0.0;

    // Divide the color by the number of samples and gamma-correct for gamma=2.0.
    auto scale = 1.0 / samples_per_pixel;
    r=pow(scale * r,gamma);  
    g=pow(scale *g,gamma);
    b=pow(scale *b,gamma);
   
  return color(r,g,b);
}
 void write_color( std::vector<vec3>& framebuffer,int pos,const color& pixel_color, int samples_per_pixel) {
    
  framebuffer[pos]=convertColor(pixel_color,samples_per_pixel,0.5);

  
};


inline double clamp(double x, double min, double max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
    };
 
 std::vector<unsigned char> convertFrameBuffer(const std::vector<vec3>& fb,int sampling_num=1){
          std::vector<unsigned char>output;
          int size=fb.size();
          output.resize(size*3);
          int count=0;
          double gamma=1.0;
           if(sampling_num!=1)gamma=0.5;
          for(const vec3& _c:fb){
            color c=convertColor(_c,sampling_num,gamma);
            output[count++]=static_cast<unsigned char>(256 * clamp(c.b(), 0.0, 0.999));
             output[count++]=static_cast<unsigned char>(256 * clamp(c.g(), 0.0, 0.999));
              output[count++]=static_cast<unsigned char>(256 * clamp(c.r(), 0.0, 0.999));
          }
            return output;
    };

    std::vector<vec3> reduceFrameBuffer(const std::vector<std::vector<vec3>>&frameBufferList){
       std::vector<vec3>ans;
        if(frameBufferList.size()==0)return ans;
        if(frameBufferList[0].size()==0)return ans;     
        int n=frameBufferList[0].size();   
        ans.resize(n);
       for(int i=0;i<n;i++){
            for(const auto& fb:frameBufferList){
                         ans[i]+=fb[i];
            }
       }
       return ans;
    };