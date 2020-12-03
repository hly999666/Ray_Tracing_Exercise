#ifndef CAMERA_H
#define CAMERA_H
#ifndef RAY_H
#include "ray.hpp"
#endif

#ifndef GENERAL_HELPER_H
#include "general_helper.hpp"
#endif
#include <cmath>
 #define M_PI 3.14159265358979323846
class camera{
    public:
    vec3 origin{0,0,0};
    vec3 target{0,0,-2};
    vec3 buttom_left{0,0,0};
    vec3 horizontal{0,0,0};
    vec3 vertical{0,0,0};
    vec3 u,v,w;
    double fov{M_PI/4};
    double aspect{2.0};
    double len_r{0.0};
    double focus_dist{1.0};
    camera()=default;
    camera(const vec3& lookForm,const vec3&  lookAt,const vec3&  up,double _fov,double _aspect,double aperture=0.0,double focus_dist=1.0):
         fov(_fov),aspect(_aspect),origin(lookForm),target(lookAt),len_r(aperture*0.5){
               double theta=fov*M_PI/180;
               double half_height=tan(theta/2);
               double half_width=half_height*aspect;
               w=unit_vector(lookForm-lookAt);
               u=unit_vector(cross(up,w));
               v=cross(w,u);
               //shift viewport to focus plane
               buttom_left=origin-(w+half_width*u+half_height*v)*focus_dist;
                  
               horizontal=2*half_width*u*focus_dist;
               vertical=2*half_height*v*focus_dist;
    };
camera(const vec3& o,const vec3& bl,const vec3& h,const vec3& v):
origin(o),buttom_left(bl),horizontal(h),vertical(v){};
  ray get_ray(double s,double t){
     //shift origin ,simulate defocus
     vec3 d_r=len_r*random_in_unit_circle();
     vec3 offset_r=u*d_r.x()+v*d_r.y();
     vec3 now_o=origin+offset_r;
     vec3 dir=buttom_left+s*horizontal+t*vertical-now_o;
      return    ray(now_o,dir);
  }
};


#endif