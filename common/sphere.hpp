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
class sphere:public hitable{
   public:
      vec3 center{0,0,0};
      float radius{0};
      material* mat_ptr;
      sphere()=default;
      sphere(vec3 c,float r,material* m):center(c),radius(r),mat_ptr(m){};
     ~sphere(){
             delete mat_ptr;
     }
      virtual bool hit(const ray&r ,float tmin,float t_max,hit_record& rc)const;
};


bool sphere::hit(const ray&r ,float t_min,float t_max,hit_record& rc)const{

  vec3 oc=r.origin()-center;
  float a=dot(r.direction(),r.direction());
  float b=2.0f*dot(oc,r.direction());
  float c=dot(oc,oc)-radius*radius;
  float d=b*b-4*a*c;
  if(d>0){
      float sd=sqrt(d);

     float t1= (-b-sd)/(2.0*a);
    float t2= (-b+sd)/(2.0*a);
     if(t1<t_max&&t1>t_min){
         rc.t=t1;
         rc.p=r.point_at_parameter(t1);
         rc.normal=(rc.p-center)/radius;
         rc.mat_ptr=mat_ptr;
         return true;
     }
    if(t2<t_max&&t2>t_min){
         rc.t=t2;
         rc.p=r.point_at_parameter(t2);
         rc.normal=(rc.p-center)/radius;
         rc.mat_ptr=mat_ptr;
         return true;
     }
  }
return false;
};


#endif