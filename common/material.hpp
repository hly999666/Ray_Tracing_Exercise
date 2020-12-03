#ifndef VEC3_H
 #include "vec3.hpp"
#endif
#ifndef RAY_H
 #include "ray.hpp"
#endif
#ifndef HITABLE_H
 #include "hitable.hpp"
#endif

#ifndef  GENERAL_HELPER_H
#include "general_helper.hpp"
#endif

#ifndef MATERIAL_H
#define  MATERIAL_H
#endif
#include <iostream>
#include <algorithm>

class material{
   public:
   virtual bool scatter(const ray&in_r,const hit_record& rc,vec3 & attenuation,ray& out_r)const=0;

};
float schlick(double cos,double rf){

    double R0=(1.0-rf)/(1.0+rf);
    R0=R0*R0;
    return R0+(1.0-R0)*pow(1.0-cos,5.0);
};
class lambertian:public material{
    public:
     random_tool* rt;
    vec3 albedo{0,0,0};
    
    lambertian()=default;
    lambertian(const vec3 a, random_tool& _rt):albedo(a),rt(&_rt){};
    virtual bool scatter(const ray&in_r,const hit_record& rc,vec3 & attenuation,ray& out_r)const{
       vec3 tar=rc.p+rc.normal+rt->random_in_unit_sphere();
       out_r=ray(rc.p,tar-rc.p);
       attenuation=albedo;
        return true;
    }

};

class metal:public material{
    public:
     random_tool* rt;
    vec3 albedo{0,0,0};
     float fuzz{0.0};
    metal()=default;
    metal(const vec3 a, float f,random_tool& _rt):albedo(a),rt(&_rt){
                         if(f<0)fuzz=0;
                         else  if(f>1)fuzz=1;
                          else  fuzz=f;
    };
    virtual bool scatter(const ray&in_r,const hit_record& rc,vec3 & attenuation,ray& out_r)const{
       vec3 refl=reflect(unit_vector(in_r.direction()),rc.normal);
       refl=refl+fuzz*rt->random_in_unit_sphere();refl.make_unit_vector();
       out_r=ray(rc.p,refl);
       attenuation=albedo;
       //check refl direction isn't pointing into surface
        return (dot(out_r.direction(),rc.normal)>0.0);
    }

};

class dielectric:public material{
     public:
      random_tool* rt;
     float ref_idx{1.5};
     dielectric()=default;
     dielectric(float rI,random_tool& _rt):ref_idx(rI),rt(&_rt){};
     virtual bool scatter(const ray& in_r,const hit_record&rc,vec3&attentunation,ray& out_r)const{
              vec3 out_n;
              vec3 reflected_dir=reflect(in_r.direction(),rc.normal);
              float n_r=1.0;
              attentunation=vec3(1.0,1.0,1.0);
            vec3 refracted_dir;
           
            bool isFromAir=false;
        
                 vec3 d_i=unit_vector(in_r.direction());
         
           
              vec3 nn=unit_vector(out_n);
            if(dot(in_r.direction(),rc.normal)>0.0){
                //from material into air 
                      out_n=-1.0*rc.normal;
                      n_r=ref_idx;
                
                      isFromAir=false;
            } else{
                     //from air into material 
                      out_n= rc.normal;
                      n_r=1.0/ref_idx;
                   
            
                        isFromAir=true;
                        
                        }
            
            if(refract(in_r.direction(),out_n,n_r,refracted_dir)){

                 out_r=ray(rc.p,refracted_dir);
            }else{
                out_r=ray(rc.p,reflected_dir) ;
                return false;
            } 
            return true;
     };
};