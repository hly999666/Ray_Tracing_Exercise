#ifndef SPHERE_H
#define SPHERE_H


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


const double PI = 3.1415926535897932385;
void get_sphere_uv(const vec3&p,double& u,double& v){
          double phi=atan2(p.z(),p.x());
          double theta=asin(p.y());
          u=1.0-(phi+PI)/(2.0*PI);
          v=(theta+PI/2)/PI;
};
class sphere:public hitable{
   public:
      vec3 center{0,0,0};
      double radius{0};
      material* mat_ptr;
      sphere()=default;
      sphere(vec3 c,double r,material* m):center(c),radius(r),mat_ptr(m){};
     ~sphere(){
             delete mat_ptr;
     }
      virtual bool hit(const ray&r ,double tmin,double t_max,hit_record& rc)const;
      virtual bool bounding_box(double t0,double t1,aabb& box)const;
      virtual double pdf_value(const vec3& o, const vec3& v) const override; 
      virtual vec3 random(const vec3& o) const override;
};


bool sphere::hit(const ray&r ,double t_min,double t_max,hit_record& rc)const{

  vec3 oc=r.origin()-center;
  double a=dot(r.direction(),r.direction());
  double b=2.0f*dot(oc,r.direction());
  double c=dot(oc,oc)-radius*radius;
  double d=b*b-4*a*c;
  if(d>0){
      double sd=sqrt(d);

     double t1= (-b-sd)/(2.0*a);
    double t2= (-b+sd)/(2.0*a);
     if(t1<t_max&&t1>=t_min){
         rc.t=t1;
         rc.p=r.point_at_parameter(t1);
         rc.normal=(rc.p-center)/radius;
         rc.mat_ptr=mat_ptr;
        get_sphere_uv((rc.p-center)/radius,	rc.u,	rc.v);
         return true;
     }
    if(t2<t_max&&t2>=t_min){
         rc.t=t2;
         rc.p=r.point_at_parameter(t2);
         rc.normal=(rc.p-center)/radius;
         rc.mat_ptr=mat_ptr;
        get_sphere_uv((rc.p-center)/radius,	rc.u,	rc.v);
         return true;
     }
  }
return false;
};
bool sphere::bounding_box(double t0,double t1,aabb& box)const{
          box=aabb(center-vec3(radius,radius,radius),center+vec3(radius,radius,radius));
         return true;
};
class moving_sphere:public hitable{
    public:
    vec3 center0,center1;
    double time0,time1;
    double radius;
    material* mat_ptr;
    moving_sphere(
    vec3 c0,vec3 c1,
    double t0,double t1,
    double r,material* m):
    center0(c0),center1(c1),
     time0(t0),time1(t1),
      radius(r),mat_ptr(m){};

    virtual bool hit(const ray& r,double tmin,double tmax,hit_record&rc)const;
    virtual bool bounding_box(double t0,double t1,aabb& box)const;
    vec3 center(double time) const;
};

vec3 moving_sphere::center(double t) const{
   return center0+((t-time0)/(time1-time0))*(center1-center0);
};

bool moving_sphere::hit(const ray&r ,double t_min,double t_max,hit_record& rc)const{
  vec3 center=this->center(r.time());
  vec3 oc=r.origin()-center;
  double a=dot(r.direction(),r.direction());
  double b=2.0f*dot(oc,r.direction());
  double c=dot(oc,oc)-radius*radius;
  double d=b*b-4*a*c;
  if(d>0){
      double sd=sqrt(d);

     double t1= (-b-sd)/(2.0*a);
    double t2= (-b+sd)/(2.0*a);
     if(t1<t_max&&t1>=t_min){
         rc.t=t1;
         rc.p=r.point_at_parameter(t1);
         rc.normal=(rc.p-center)/radius;
         rc.mat_ptr=mat_ptr;
           get_sphere_uv((rc.p-center)/radius,	rc.u,	rc.v);
         return true;
     }
    if(t2<t_max&&t2>=t_min){
         rc.t=t2;
         rc.p=r.point_at_parameter(t2);
         rc.normal=(rc.p-center)/radius;
         rc.mat_ptr=mat_ptr;
           get_sphere_uv((rc.p-center)/radius,	rc.u,	rc.v);
         return true;
     }
  }
return false;
};

bool moving_sphere::bounding_box(double t0,double t1,aabb& box)const{
         auto center0=center(t0);
         auto center1=center(t1);
          aabb  box0(center0-vec3(radius,radius,radius),center0+vec3(radius,radius,radius));
          aabb box1(center1-vec3(radius,radius,radius),center1+vec3(radius,radius,radius));
          box=aabb_union(box0,box1);
         return true;
};

double sphere::pdf_value(const vec3& o, const vec3& v) const { 
 
    return  0.0; 
} 
 
vec3 sphere::random(const point3& o) const { 
    return vec3(0.0,0.0,0.0);
}

#endif