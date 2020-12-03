#ifndef VEC3_H
 #include "vec3.hpp"
#endif
#ifndef RAY_H
 #define RAY_H
#endif
class ray{
    public:
    vec3 A;
    vec3 B;
    ray(){}
    ray(const vec3&a ,const vec3&b):A(a),B(b){};
    vec3 origin() const{return A;};
    vec3 direction() const{return B;};
    vec3 point_at_parameter(double t)const{return A+t*B;};

};