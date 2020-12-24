 #ifndef TRANSFORM_H
 #define TRANSFORM_H

 
 #ifndef HITABLE_H
 #include "hitable.hpp"
#endif

 #ifndef VEC3_H
 #include "vec3.hpp"
#endif

 #ifndef RAY_H
 #include "ray.hpp"
#endif

  #ifndef RAY_H
 #include "ray.hpp"
#endif
#ifndef GENERAL_HELPER_H
 #include "general_helper.hpp"
#endif

#include  <cfloat> 
class translate:public hitable{
   public:
   vec3 offset{0,0,0};
   hitable* ptr{nullptr};
   translate()=default;
   translate(hitable*p,const vec3& _offset):ptr(p),offset(_offset){};
   virtual bool hit(const ray& r,double tmin,double tmax,hit_record& rc)const;
   virtual bool bounding_box(double t0,double t1,aabb& box)const;
};
bool translate::hit(const ray& r,double tmin,double tmax,hit_record& rc)const{
    ray r_moved(r.origin()-offset,r.direction(),r.time());
    if(ptr->hit(r_moved,tmin,tmax,rc)){
           rc.p+=offset;   
           return true;      
    }else return false;
     return false;
};

bool translate::bounding_box(double t0,double t1,aabb& box)const{
             if(ptr->bounding_box(t0,t1,box)){
                box=aabb(box.min()+offset,box.max()+offset);
                return true;
             }else return false;

};

class rotate_y:public hitable{
   public:
   hitable* ptr{nullptr};
   double sin_t{0.0};
   double cos_t{0.0};
   bool hasBox{false};
   aabb bbox{vec3(0,0,0),vec3(0,0,0)};
   rotate_y()=default;
   rotate_y(hitable*p,double angle);
   virtual bool hit(const ray& r,double tmin,double tmax,hit_record& rc)const;
   virtual bool bounding_box(double t0,double t1,aabb& box)const;
};
rotate_y::rotate_y(hitable *p,double angle):ptr(p){
   double rad=degToRad(angle);
   sin_t=sin(rad);cos_t=cos(rad);
   hasBox=ptr->bounding_box(0,1,bbox);
   vec3 min(DBL_MIN,DBL_MIN,DBL_MIN);
   vec3 max(DBL_MAX,DBL_MAX,DBL_MAX);
   // find aabb
   for(int i=0;i<2;i++){
     for(int  j=0;j<2;j++){
        for(int  k=0;k<2;k++){
              double x=i*bbox.max().x()+(1-i)*bbox.max().x();
              double y=j*bbox.max().y()+(1-j)*bbox.max().y();
              double z=k*bbox.max().z()+(1-k)*bbox.max().z();
              double neo_x=cos_t*x+sin_t*z;
              double neo_z=-sin_t*x+cos_t*z;
              vec3 neo_p(neo_x,y,neo_z);
              for(int c=0;c<3;c++){
                      if(neo_p[c]>max[c])max[c]=neo_p[c];
                      if(neo_p[c]<min[c])min[c]=neo_p[c];
              }
       }
     }
   }
   bbox=aabb(min,max);
}
bool rotate_y::hit(const ray& r,double tmin,double tmax,hit_record& rc)const{
   vec3 o=r.origin();
   vec3 dir=r.direction();
   o[0]=cos_t*r.origin()[0]-sin_t*r.origin()[2];
   o[2]=sin_t*r.origin()[0]+cos_t*r.origin()[2];
   dir[0]=cos_t*r.direction()[0]-sin_t*r.direction()[2];
   dir[2]=sin_t*r.direction()[0]+cos_t*r.direction()[2];
   ray r_rotated(o,dir,r.time());
   if(ptr->hit(r_rotated,tmin,tmax,rc)){
             vec3 p=rc.p;
             vec3 normal=rc.normal;
             p[0]=cos_t*rc.p[0]+sin_t*rc.p[2];
             p[2]=-sin_t*rc.p[0]+cos_t*rc.p[2];
             normal[0]=cos_t*rc.normal[0]+sin_t*rc.normal[2];
             normal[2]=-sin_t*rc.normal[0]+cos_t*rc.normal[2];
             rc.p=p;
             rc.normal=normal;
             return true;
   }else return false;
};

bool rotate_y::bounding_box(double t0,double t1,aabb& box)const{
           if(hasBox){
             box=bbox;
             return true;
           }else return false;
};
#endif