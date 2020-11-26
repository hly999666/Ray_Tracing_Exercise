#ifndef HITABLE_H
#define HITABLE_H
#ifndef RAY_H
#include "ray.hpp"
#endif
#ifndef VEC3_H
 #include "vec3.hpp"
#endif
struct hit_record{
    float t;
    vec3 p;
    vec3 normal;
};

class hitable{

public:
//pure virtual method ,must be children class overwritten
virtual bool hit(const ray& r ,float t_min,float t_max,hit_record& rec)const =0;


};





#endif