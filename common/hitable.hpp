#ifndef HITABLE_H
#define HITABLE_H
#ifndef RAY_H
#include "ray.hpp"
#endif
#ifndef VEC3_H
 #include "vec3.hpp"
#endif
class material;
struct hit_record{
    double t;
    vec3 p;
    vec3 normal;
    material* mat_ptr;
};

class hitable{

public:
//pure virtual method ,must be children class overwritten
virtual bool hit(const ray& r ,double t_min,double t_max,hit_record& rec)const =0;


};





#endif