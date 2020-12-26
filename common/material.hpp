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
 
#ifndef TEXTURE_H
#include  "texture.hpp"
#endif
#include <iostream>
#include <algorithm>
#ifndef MATERIAL_H
#define MATERIAL_H
class material{
   public:
   virtual bool scatter(const ray&in_r,const hit_record& rc,vec3 & attenuation,ray& out_r)const=0;
   virtual vec3 emitted(double u,double v,const vec3&p)const{
        return vec3(0.0,0.0,0.0);
    }
};

double schlick(double cos,double rf){

    double R0=(1.0-rf)/(1.0+rf);
    R0=R0*R0;
    return R0+(1.0-R0)*pow(1.0-cos,5.0);
};
class lambertian:public material{
    public:
  
    texture* albedo{nullptr};
    
    lambertian()=default;
    lambertian(texture* a):albedo(a){};
    virtual bool scatter(const ray&in_r,const hit_record& rc,vec3 & attenuation,ray& out_r)const{
       vec3 tar=rc.p+rc.normal+random_in_unit_sphere();
       out_r=ray(rc.p,tar-rc.p);
       attenuation=albedo->value(rc.u,rc.v,rc.p);
        return true;
    }

};

class metal:public material{
    public:
 
    vec3 albedo{0,0,0};
     double fuzz{0.0};
    metal()=default;
    metal(const vec3 a, double f=0.0):albedo(a){
                         if(f<0)fuzz=0.0;
                         else  if(f>1)fuzz=1.0;
                          else  fuzz=f;
    };
    virtual bool scatter(const ray&in_r,const hit_record& rc,vec3 & attenuation,ray& out_r)const{
       vec3 refl=reflect(unit_vector(in_r.direction()),rc.normal);
       refl=refl+fuzz*random_in_unit_sphere();refl.make_unit_vector();
       out_r=ray(rc.p,refl);
       attenuation=albedo;
       //check refl direction isn't pointing into surface
        return (dot(out_r.direction(),rc.normal)>0.0);
    }

};

class dielectric:public material{
     public:
 
     double ref_idx{1.5};
     dielectric()=default;
     dielectric(double rI ):ref_idx(rI) {};
     virtual bool scatter(const ray& in_r,const hit_record&rc,vec3&attentunation,ray& out_r)const{
              vec3 out_n;
              vec3 reflected_dir=reflect(in_r.direction(),rc.normal);
              double n_r=1.0;
              attentunation=vec3(1.0,1.0,1.0);
            vec3 refracted_dir;
           
            bool isFromAir=false;
        
                 vec3 d_i=unit_vector(in_r.direction());
         
           double cos;
           double refl_ratio;
              vec3 nn=unit_vector(out_n);
            if(dot(in_r.direction(),rc.normal)>0.0){
                
                      out_n=-1.0*rc.normal;
                       n_r=ref_idx;
                      cos=ref_idx*dot(in_r.direction(),rc.normal)/in_r.direction().length();
                      isFromAir=false;
            } else{
                   
                      out_n= rc.normal;
                      n_r=1.0/ref_idx;
                   
                    cos=-1.0*dot(in_r.direction(),rc.normal)/in_r.direction().length();
                        isFromAir=true;
                        
                        }
            
            if(refract(in_r.direction(),out_n,n_r,refracted_dir)){

                refl_ratio=schlick(cos,ref_idx);
            }else{
                out_r=ray(rc.p,reflected_dir) ;
                refl_ratio=1.0;
            } 
              if(random_double()<refl_ratio){
                 out_r=ray(rc.p,reflected_dir) ;
               
            }else{
                 out_r=ray(rc.p,refracted_dir);
             
           
            } 
            return true;
     };
};


class diffuse_light:public material{
     public:
     texture* emit{nullptr};
     diffuse_light()=default;
     diffuse_light(texture* a):emit(a){};
     virtual bool scatter(const ray& in_r,const hit_record& rc,vec3& atten,ray& o_r)const{return false;};
     virtual vec3 emitted(double u,double v,const vec3&p)const{
          return emit->value(u,v,p);
    }
};

class isotropic : public material {
    public:
        texture*  albedo{nullptr};
        isotropic()=default;
        isotropic(color c) : albedo(new constant_texture(c)){};
        isotropic(texture* a) : albedo(a) {};

        virtual bool scatter(
            const ray& in_r, const hit_record& rc, color& atten, ray& out_r
        ) const   {
            out_r = ray(rc.p, random_in_unit_sphere(), in_r.time());
            atten = albedo->value(rc.u, rc.v, rc.p);
            return true;
        } 
};
#endif