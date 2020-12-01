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
class material{
   public:
   virtual bool scatter(const ray&in_r,const hit_record& rc,vec3 & attenuation,ray& out_r)const=0;

};

class lambertian:public material{
    public:
    vec3 albedo{0,0,0};
     random_tool* rt;
    lambertian()=default;
    lambertian(const vec3 a):albedo(a){};
    virtual bool scatter(const ray&in_r,const hit_record& rc,vec3 & attenuation,ray& out_r)const{
    vec3 tar=rc.p+rc.normal;

        return true;
    }

};