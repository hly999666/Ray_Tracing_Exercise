#ifndef HITABLE_H
#define HITABLE_H
#ifndef RAY_H
#include "ray.hpp"
#endif
#ifndef VEC3_H
 #include "vec3.hpp"
#endif

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
class material;
class hit_record{
    public:
    double t;
    vec3 p;
    vec3 normal;
    double u{0.0};
    double v{0.0};
    material* mat_ptr{nullptr};
    hit_record()=default;
    /* ~hit_record(){
        delete mat_ptr;
    } */
};


class hitable{

public:
//pure virtual method ,must be children class overwritten

virtual bool hit(const ray& r ,double t_min,double t_max,hit_record& rec)const =0;
virtual bool bounding_box(double t0,double t2,aabb&box)const=0;
virtual double pdf_value(const vec3& o, const vec3& v) const { return 0.0;}; 
virtual vec3 random(const vec3& o) const { return vec3(0, 0, 0);};
};


class flip_normals:public hitable{
    public:
    hitable* ptr{nullptr};
    flip_normals(hitable* p):ptr(p){};
    virtual bool hit(const ray& r,double t_min,double t_max,hit_record&rc)const{
         if(ptr->hit(r,t_min,t_max,rc)){
              rc.normal=-1.0*rc.normal;
              return true;
         }else return false; 
    };
    virtual bool bounding_box(double t0,double t1,aabb& box)const{
        return ptr->bounding_box(t0,t1,box);
    }
    virtual double  pdf_value(const vec3& o, const vec3& v)const override{
          return ptr->pdf_value(o,v);
    };
    virtual vec3 random(const vec3& o) const override{
       return ptr->random(o);
     };
};


#endif