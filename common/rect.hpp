#ifndef RECT_H
#define RECT_H


#ifndef HITABLE_H
#include "hitable.hpp"
#endif
 
 #ifndef VEC3_H
 #include "vec3.hpp"
#endif
#ifndef MATERIAL_H
 #include "material.hpp"
#endif

#ifndef BVH_H
#include "bvh.hpp"
#endif

#include <cmath>

class xy_rect:public hitable{
  public:
  material* mp{nullptr};
  double x0{0.0}; double x1{0.0};
  double y0{0.0}; double y1{0.0};
  double z{0.0};
     xy_rect()=default;
     xy_rect(double _x0,double _x1,double _y0,double _y1,double _z,material* _m):
       x0(_x0),  x1(_x1),   y0(_x0),  y1(_x1), z(_z),mp(_m){};
    virtual bool hit(const ray& r,double t0,double t1,hit_record&rc)const;
    virtual bool bounding_box(double t0,double t1,aabb&box)const{
        box=aabb(vec3(x0,y0,z-0.0001),vec3(x1,y1,z+0.0001));
        return true;
    };
};

bool xy_rect::hit(const ray& r,double t0,double t1,hit_record&rc)const{
       double t=(z-r.origin().z())/r.direction().z();
      if(t<t0||t>t1)return false;
    double x=r.origin().x()+t*r.direction().x();
    double y=r.origin().y()+t*r.direction().y();
    if(x<x0||x>x1||y<y0||y>y1)return false;
 
     rc.u=(x-x0)/(x1-x0);
     rc.v=(y-y0)/(y1-y0); 
     rc.t=t;
     rc.mat_ptr=mp;
     rc.p=r.point_at_parameter(t);
     rc.normal=vec3(0,0,1);
     return true;
};













#endif