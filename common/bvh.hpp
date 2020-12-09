#ifndef BVH_H
#define BVH_H
 #ifndef VEC3_H
 #include "vec3.hpp"
#endif
 #ifndef RAY_H
 #include "ray.hpp"
#endif
#ifndef HITABLE_H
#include "hitable.hpp"
#endif

#include <algorithm>
 
inline double dmin(double a,double b){return a<b?a:b;};
inline double dmax(double a,double b){return a>b?a:b;};
class aabb{
    public:
    vec3 _min{0,0,0};
    vec3 _max{0,0,0};
    aabb()=default;
    aabb(const vec3& a,const vec3& b):_min(a),_max(b){};
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
};
aabb aabb_union(const aabb& a,const aabb& b){
       vec3 _min(dmin(a.min().x(),b.min().x()),
                         dmin(a.min().y(),b.min().y()),
                         dmin(a.min().z(),b.min().z()));
        vec3 _max(dmax(a.max().x(),b.max().x()),
                          dmax(a.max().y(),b.max().y()),
                          dmax(a.max().z(),b.max().z()));
        return aabb(_min,_max);
};

class bvh_node:public hitable{
     public:
     hitable* left;
     hitable* right;
     aabb box;
     bvh_node()=default;
     bvh_node(hitable** l,int n,double time0,double time1);
     virtual bool hit(const ray& r,double tmin,double tmax,hit_record&rc)const;
     virtual bool bounding_box(double t1,double t2,aabb& box)const;
     
};
bool bvh_node::hit(const ray& r,double tmin,double tmax,hit_record&rc)const{
    return false;
};
bool bvh_node::bounding_box(double t1,double t2,aabb& box)const{
    return false;
};

void build_bvh_tree(bvh_node* target,hitable** l,int n,double time0,double time1){
     
};

bvh_node::bvh_node(hitable** l,int n,double time0,double time1){

    build_bvh_tree(this,l,n,time0,time1);
};
#endif