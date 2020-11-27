#ifndef CAMERA_H
#define CAMERA_H
#ifndef RAY_H
#include "ray.hpp"
#endif

class camera{
    public:
    vec3 origin{0,0,0};
    vec3 top_left{0,0,0};
    vec3 horizontal{0,0,0};
    vec3 vertical{0,0,0};
    camera()=default;
camera(const vec3& o,const vec3& tl,const vec3& h,const vec3& v):
origin(o),top_left(tl),horizontal(h),vertical(v){};
  ray get_ray(float u,float v){
      vec3 dir=top_left+u*horizontal+v*vertical;
      return    ray(origin,dir);
  }
};


#endif