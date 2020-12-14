#ifndef VEC3_H
 #include "vec3.hpp"
#endif
#ifndef PERLIN_NOISE_H
#include "perlin_noise.hpp" 
#endif
#ifndef TEXTURE_H
#define  TEXTURE_H
 #include <cmath>
class texture{
    public:
    virtual vec3 value(double u,double v,const vec3&p)const=0;
};

class constant_texture:public texture{
     public:
     vec3 color{0.0,0.0,0.0};
     constant_texture()=default;
     constant_texture(vec3 c):color(c){};
     virtual vec3 value(double u,double v,const vec3&p)const{
         return  color;
     };
};

class checker_texture:public texture{
     public:
     texture* tex_0;
     texture* tex_1;
     double unit{10};
     checker_texture()=default;
     checker_texture(texture* t0,texture* t1,double u=10.0):tex_0(t0),tex_1(t1),unit{u}{};
     virtual vec3 value(double u,double v,const vec3&p)const{
         double s3=sin(unit*p.x())*sin(unit*p.y())*sin(unit*p.z());
         if(s3<0){
             return tex_0->value(u,v,p);
         }else {
              return tex_1->value(u,v,p);
         }
     };
};
class noise_texture:public texture{
    public:
    perlin noise;
    double freq{4.0};
    noise_texture()=default;
    noise_texture(double f):freq(f){};
    virtual vec3 value(double u,double v,const vec3&p)const{
        return vec3(1.0,1.0,1.0)*noise.noise(p,freq);
    };
};
#endif