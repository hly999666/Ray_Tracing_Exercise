 
#include <cmath>
#include <iostream>
#include <string>
#include <stdlib.h>

#ifndef VEC3_H
 #define VEC3_H
#endif
 
using std::sqrt;
using std::fabs;
class vec3;
inline vec3 operator+ (const vec3&v1,const vec3&v2);
inline vec3 operator- (const vec3&v1,const vec3&v2);
inline vec3 operator* (const vec3&v1,const vec3&v2);
inline vec3 operator/ (const vec3&v1,const vec3&v2);
inline vec3 operator* (const vec3&v1,const float& t);
inline vec3 operator/ (const vec3&v1,const float& t);
inline vec3 operator* (const float& t,const vec3&v1);
inline vec3 operator/ (const float& t,const vec3&v1);
class vec3 {
 
    public:
        float e[3];
        bool isUnit;
        vec3() : e{0,0,0},isUnit(false) {};
        vec3(float e0, float e1, float e2) : e{e0, e1, e2},isUnit(false)  {};
        vec3(const vec3&v2){
          e[0]=v2.e[0];e[1]=v2.e[1];e[2]=v2.e[2];
      }
         inline float x() const{return e[0];}
         inline float y() const{return e[1];}
         inline float z() const{return e[2];}
         inline float r() const{return e[0];}
         inline float g() const{return e[1];}
         inline float b() const{return e[2];}
       inline const vec3& operator+()const{return *this;}
       //nline  vec3& operator+(){return *this;}
       inline  vec3 operator-()const{return  vec3(-e[0],-e[1],-e[2]);}
       inline  float operator[](int i)const{return e[i];}
       inline  float& operator[](int i){return e[i];}
      inline  void operator=(const vec3&v2){
          e[0]=v2.e[0];e[1]=v2.e[1];e[2]=v2.e[2];
      }
       inline vec3&operator+=(const vec3&v2){
           *this= *this+v2;return *this;
       };
        inline vec3&operator-=(const vec3&v2){
             *this= *this-v2;return *this;
        };
        inline vec3&operator*=(const vec3&v2){
             *this= *this*v2;return *this;
        };
        inline vec3&operator/=(const vec3&v2){
              *this= *this/v2;return *this;
        };
     
        inline vec3&operator*=(const float t){
                       e[0]*=t;e[1]*=t; e[2]*=t;
                       return *this;
           };
        inline vec3&operator/=(const float t){
                       e[0]/=t; e[1]/=t; e[2]/=t;
                       return *this;
          };
          inline float length()const{
              return sqrt( e[0]* e[0]+e[1]* e[1]+e[2]* e[2]);
          }
         inline float squared_length()const{
              return   e[0]* e[0]+e[1]* e[1]+e[2]* e[2];
          }
          inline void make_unit_vector() {
             if(isUnit)return;
             (*this)/=squared_length() ;
                isUnit=true;
          }
          
};

inline float dot(const vec3&v1,const vec3&v2){
    return v1.e[0]*v2.e[0]+ v1.e[1]*v2.e[1]+ v1.e[2]*v2.e[2];
}
 inline vec3 cross(const vec3&a,const vec3&b){
    return vec3(
  a.y()*b.z()-a.z()*b.y(),
  a.z()*b.x()-a.x()*b.z(),
  a.x()*b.y()-a.y()*b.x()
   );
}
inline vec3 unit_vector(const vec3&v){
    return v/v.length();
}

inline vec3 operator+ (const vec3&v1,const vec3&v2){
    return vec3(
        v1.x()+v2.x(),
        v1.y()+v2.y(),
        v1.z()+v2.z()
    );
};
inline vec3 operator- (const vec3&v1,const vec3&v2){
    return vec3(
        v1.x()-v2.x(),
        v1.y()-v2.y(),
        v1.z()-v2.z()
    );
};
inline vec3 operator* (const vec3&v1,const vec3&v2){
  return vec3(
        v1.x()*v2.x(),
        v1.y()*v2.y(),
        v1.z()*v2.z()
    );

};
inline vec3 operator/ (const vec3&v1,const vec3&v2){
    return vec3(
        v1.x()/v2.x(),
        v1.y()/v2.y(),
        v1.z()/v2.z()
    );
};
inline vec3 operator* (const vec3&v1,const float& t){
      return vec3(
        v1.x()*t,
        v1.y()*t,
        v1.z()*t
    );
};
inline vec3 operator/ (const vec3&v1,const float& t){
     return vec3(
        v1.x()/t,
        v1.y()/t,
        v1.z()/t
    );
};
inline vec3 operator* (const float& t,const vec3&v1){
       return v1*t;
};
inline vec3 operator/ (const float& t,const vec3&v1){
      return v1/t;
};

inline vec3 reflect(const vec3& v,const vec3& n){
     vec3 nn{n};nn.make_unit_vector();
     return v-2.0*dot(v,nn)*nn;
}

bool refract(const vec3& v,const vec3&n,float n_r,vec3& refracted){
         //default v is pointing into surface
        vec3 uv=unit_vector(v)*-1.0;
         float dt=fmin(dot(uv,n),1.0);
         float D=1.0-n_r*n_r*(1.0-dt*dt);
      if(D>0.0){
           refracted=n_r*(n*dt-uv)-n*sqrt(D);
            return true;
      }else return false;
}

