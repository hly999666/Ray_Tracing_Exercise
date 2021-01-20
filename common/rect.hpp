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
       x0(_x0),  x1(_x1),   y0(_y0),  y1(_y1), z(_z),mp(_m){};
    virtual bool hit(const ray& r,double t0,double t1,hit_record&rc)const;
    virtual bool bounding_box(double t0,double t1,aabb&box)const{
        box=aabb(vec3(x0,y0,z-0.0001),vec3(x1,y1,z+0.0001));
        return true;
    };
};

class xz_rect:public hitable{
  public:
  material* mp{nullptr};
  double x0{0.0}; double x1{0.0};
  double z0{0.0}; double z1{0.0};
  double y{0.0};
     xz_rect()=default;
     xz_rect(double _x0,double _x1,double _z0,double _z1,double _y,material* _m):
       x0(_x0),  x1(_x1),   z0(_z0),  z1(_z1), y(_y),mp(_m){};
    virtual bool hit(const ray& r,double t0,double t1,hit_record&rc)const;
    virtual bool bounding_box(double t0,double t1,aabb&box)const {
        box=aabb(vec3(x0,y-0.001,z0),vec3(x1,y+0.001,z1));
        return true;
    };
     virtual double pdf_value(const vec3& o, const vec3& v) const override;
     virtual vec3 random(const vec3&  o) const override;
};
  double xz_rect::pdf_value(const vec3& o, const vec3& v)const
{ 
            hit_record rec; 
            if (!this->hit(ray(o, v), 0.001, inf, rec))return 0.0; 
            auto area = (x1-x0)*(z1-z0); 
            auto distance_squared = rec.t * rec.t * v.squared_length(); 
            auto cos = abs(dot(v, rec.normal) / v.length()); 
 
            return distance_squared / (cos * area); 
  };
vec3 xz_rect::random(const vec3&  o)const
  { 
            auto random_point = vec3(random_double(x0,x1), y, random_double(z0,z1)); 
            return random_point - o; 
      };
class yz_rect:public hitable{
  public:
  material* mp{nullptr};
  double y0{0.0}; double y1{0.0};
  double z0{0.0}; double z1{0.0};
  double x{0.0};
     yz_rect()=default;
     yz_rect(double _y0,double _y1,double _z0,double _z1,double _x,material* _m):
       y0(_y0), y1(_y1),  z0(_z0),  z1(_z1), x(_x),mp(_m){};
    virtual bool hit(const ray& r,double t0,double t1,hit_record&rc)const;
    virtual bool bounding_box(double t0,double t1,aabb&box)const{
        box=aabb(vec3(x-0.001,y0,z0),vec3(x+0.001,y1,z1));
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

bool xz_rect::hit(const ray& r,double t0,double t1,hit_record&rc)const {
       double t=(y-r.origin().y())/r.direction().y();
      if(t<t0||t>t1)return false;
    double x=r.origin().x()+t*r.direction().x();
    double z=r.origin().z()+t*r.direction().z();
    if(x<x0||x>x1||z<z0||z>z1)return false;
 
     rc.u=(x-x0)/(x1-x0);
     rc.v=(z-z0)/(z1-z0); 
     rc.t=t;
     rc.mat_ptr=mp;
     rc.p=r.point_at_parameter(t);
     rc.normal=vec3(0,1,0);
     return true;
};

bool yz_rect::hit(const ray& r,double t0,double t1,hit_record&rc)const{
    double t=(x-r.origin().x())/r.direction().x();
    if(t<t0||t>t1)return false;
    double y=r.origin().y()+t*r.direction().y();
    double z=r.origin().z()+t*r.direction().z();
    if(y<y0||y>y1||z<z0||z>z1)return false;
     rc.u=(y-y0)/(y1-y0);
     rc.v=(z-z0)/(z1-z0); 
     rc.t=t;
     rc.mat_ptr=mp;
     rc.p=r.point_at_parameter(t);
     rc.normal=vec3(1,0,0);
     return true;
};














#endif