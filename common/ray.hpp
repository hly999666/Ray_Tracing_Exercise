#ifndef VEC3_H
 #include "vec3.hpp"
#endif
#ifndef RAY_H
 #define RAY_H
#endif
class ray{
    public:
    vec3 A{0.0,0.0,0.0};
    vec3 B{0.0,0.0,0.0};
    double _time{0.0};
    ray()=default;
    ray(const vec3&a ,const vec3&b,double _ti=0.0):A(a),B(b),_time(_ti){};
    vec3 origin() const{return A;};
    vec3 direction() const{return B;};
    vec3 point_at_parameter(double t)const{return A+t*B;};
    double time()const{return _time;}
};