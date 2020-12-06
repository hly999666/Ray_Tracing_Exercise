#ifndef BVH_H
#define BVH_H
 #ifndef VEC3_H
 #include "vec3.hpp"
#endif
 #ifndef RAY_H
 #include "ray.hpp"
#endif
#include <algorithm>
inline double dmin(double a,double b){return a<b?a:b;}
inline double dmax(double a,double b){return a>b?a:b;}
class aabb{
    public:
    vec3 _min{0,0,0};
    vec3 _max{0,0,0};
    aabb()=default;
    vec3 min()const {return _min;}
    vec3 max()const {return _max;}
    bool hit(const ray& r,double tmin,double tmax)const;

};
inline bool aabb::hit(const ray& r,double tmin,double tmax)const{
         for(int i=0;i<3;i++){
                   double invD=1.0/r.direction()[i];
                   const vec3& o=r.origin();
                   double t0=(_min[i]-o[i])*invD;
                   double t1=(_max[i]- o[i])*invD;
                   if(invD<0.0)std::swap(t0,t1);
            tmin=dmax(t0,tmin);
            tmax=dmin(t1,tmax);
            if(tmax<=tmin)return false;
         }

        return true;
} 

#endif