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
#ifndef PDF_H
#include  "pdf.hpp"
 #endif
#include <iostream>
#include <algorithm>
#ifndef MATERIAL_H
#define MATERIAL_H

class scatter_record { 
    public:
    ray specular_ray; 
    bool is_specular; 
    vec3 attenuation; 
    pdf pdf; 
    scatter_record()=default;
    ~scatter_record()=default;
};
 
class material{
   public:
   virtual bool scatter(
       const ray&in_r,
       const hit_record& rc,
       scatter_record& s_rc)const=0;
   virtual double scattering_pdf(
       const ray&in_r,
       const hit_record&rc,
       const ray& scattered)const{return 0.0;}
   virtual vec3 emitted(
       const ray& r_in,
       const hit_record&rc,
       double u,double v,
       const vec3&p)const{
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

      virtual double scattering_pdf(const ray&in_r,const hit_record&rc,const ray& out_r)const{
          double cos=dot(rc.normal,unit_vector(out_r.direction()));
          if(cos<0.0)  cos=0; 
          return cos/pi;
    }
    virtual bool scatter(
    const ray&in_r,
    const hit_record& rc,
    scatter_record& s_rc )const{
  /*    vec3 dir=random_in_hemisphere(rc.normal); */
  /*    do{
       dir=random_in_unit_sphere();
     }while(dot(dir,rc.normal)<0.0); */
 
    /*    out_r=ray(rc.p,unit_vector(dir),in_r.time());
       attenuation=albedo->value(rc.u,rc.v,rc.p);
        pdf=0.5/pi; */
        //sampling with cos 
        /* vec3 tar=rc.p+rc.normal+random_in_unit_sphere();
        out_r=ray(rc.p,unit_vector(tar-rc.p),in_r.time());
           attenuation=albedo->value(rc.u,rc.v,rc.p);
           pdf=dot(rc.normal,out_r.direction())/pi; */
            vec3 attenuation;
              ray out_r;
              double pdf; 

            onb uvw;
            uvw.build_from_w(rc.normal);
            vec3 dir=uvw.local(random_cosine_direction());
            out_r=ray(rc.p,unit_vector(dir),in_r.time());
            attenuation=albedo->value(rc.u,rc.v,rc.p);
            pdf=dot(uvw.w(),out_r.direction())/pi;

            s_rc.is_specular=false;
            s_rc.attenuation=attenuation;
            s_rc.pdf=cosine_pdf(rc.normal);
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
    virtual bool scatter(
        const ray&in_r,
        const hit_record& rc,
         scatter_record& s_rc)const{
             vec3 attenuation;
              ray out_r;
              double pdf; 
       vec3 refl=reflect(unit_vector(in_r.direction()),rc.normal);
       refl=refl+fuzz*random_in_unit_sphere();
       refl.make_unit_vector();
       out_r=ray(rc.p,refl);
       attenuation=albedo;
       //check refl direction isn't pointing into surface

        
            s_rc.specular_ray = ray(rc.p, refl); 
            s_rc.attenuation = albedo; 
            s_rc.is_specular = true; 
            
        return (dot(out_r.direction(),rc.normal)>0.0);
    }

};

class dielectric:public material{
     public:
 
     double ref_idx{1.5};
     dielectric()=default;
     dielectric(double rI ):ref_idx(rI) {};
     virtual bool scatter(
         const ray& in_r,
         const hit_record&rc,
         scatter_record& s_rc)const{
              vec3 attentunation;
              ray out_r;
              double pdf; 
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

         s_rc.is_specular = true; 
         s_rc.attenuation = color(1.0, 1.0, 1.0);
         s_rc.specular_ray=out_r;
            return true;
     };
};


class diffuse_light:public material{
     public:
     texture* emit{nullptr};
     diffuse_light()=default;
     diffuse_light(texture* a):emit(a){};
     virtual bool scatter(const ray& in_r,const hit_record& rc,scatter_record& s_rc)const{return false;};
     virtual vec3 emitted(const ray& r_in,const hit_record&rc, double u,double v,const vec3&p)const{
         if(dot(rc.normal,r_in.direction())<0.0) return emit->value(u,v,p);
         else return vec3(0.0,0.0,0.0);
    }
};

class isotropic : public material {
    public:
        texture*  albedo{nullptr};
        isotropic()=default;
        isotropic(color c) : albedo(new constant_texture(c)){};
        isotropic(texture* a) : albedo(a) {};
           
        virtual bool scatter(
            const ray& in_r, const hit_record& rc, scatter_record& s_rc) const   {
                color atten;
                 ray out_r;
                 double pdf;
            out_r = ray(rc.p, random_in_unit_sphere(), in_r.time());
            atten = albedo->value(rc.u, rc.v, rc.p);
            return true;
        } 
};
#endif