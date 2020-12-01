#ifndef VEC3_H
 #include "vec3.hpp"
#endif
#ifndef RAY_H
 #include "ray.hpp"
#endif
#ifndef HITABLE_H
 #include "hitable.hpp"
#endif

#ifndef RANDOM_TOOL_H
#include "random_tool.hpp"
#endif

#ifndef MATERIAL_H
#define  MATERIAL_H
#endif
#include <iostream>
#include <algorithm>

class material{
   public:
   virtual bool scatter(const ray&in_r,const hit_record& rc,vec3 & attenuation,ray& out_r)const=0;

};

class lambertian:public material{
    public:
     random_tool* rt;
    vec3 albedo{0,0,0};
    
    lambertian()=default;
    lambertian(const vec3 a, random_tool& _rt):albedo(a),rt(&_rt){};
    virtual bool scatter(const ray&in_r,const hit_record& rc,vec3 & attenuation,ray& out_r)const{
       vec3 tar=rc.p+rc.normal+rt->random_in_unit_sphere();
       out_r=ray(rc.p,tar-rc.p);
       attenuation=albedo;
        return true;
    }

};

class metal:public material{
    public:
     random_tool* rt;
    vec3 albedo{0,0,0};
     float fuzz;
    metal()=default;
    metal(const vec3 a, float f,random_tool& _rt):albedo(a),rt(&_rt){
                         if(f<0)fuzz=0;
                         else  if(f>1)fuzz=1;
                          else  fuzz=f;
    };
    virtual bool scatter(const ray&in_r,const hit_record& rc,vec3 & attenuation,ray& out_r)const{
       vec3 refl=reflect(unit_vector(in_r.direction()),rc.normal);
       refl=refl+fuzz*rt->random_in_unit_sphere();refl.make_unit_vector();
       out_r=ray(rc.p,refl);
       attenuation=albedo;
       //check refl direction isn't pointing into surface
        return (dot(out_r.direction(),rc.normal)>0.0);
    }

};