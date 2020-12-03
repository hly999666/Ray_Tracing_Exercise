#ifndef CAMERA_H
#define CAMERA_H
#ifndef RAY_H
#include "ray.hpp"
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
    float fov{M_PI/4};
    float aspect{2.0};
    camera()=default;
    camera(const vec3& lookForm,const vec3&  lookAt,const vec3&  up,float _fov,float _aspect):
         fov(_fov),aspect(_aspect),origin(lookForm),target(lookAt){
               float theta=fov*M_PI/180;
               float half_height=tan(theta/2);
               float half_width=half_height*aspect;
               w=unit_vector(lookForm-lookAt);
               u=unit_vector(cross(up,w));
               v=cross(w,u);
               buttom_left=origin-w-half_width*u-half_height*v;
                 
               horizontal=2*half_width*u;
               vertical=2*half_height*v;

    };
camera(const vec3& o,const vec3& bl,const vec3& h,const vec3& v):
origin(o),buttom_left(bl),horizontal(h),vertical(v){};
  ray get_ray(float u,float v){
      vec3 dir=buttom_left+u*horizontal+v*vertical-origin;
      return    ray(origin,dir);
  }
};


#endif