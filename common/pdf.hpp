#ifndef VEC3_H
  #include "vec3.hpp"
#endif
#ifndef HITABLE_H
  #include "hitable.hpp"
#endif
#ifndef GENERAL_HELPER_H  
  #include "general_helper.hpp"
#endif
#ifndef RECT_H
  #include "rect.hpp"
#endif
#ifndef PDF_H
 #define PDF_H
inline double pdf_uniform_on_sphere(const vec3&p){
    return 1/(4*M_PI);
};

class pdf{
    public:
       virtual ~pdf(){};
       virtual double value(const vec3& dir)const=0;
       virtual vec3 generate()const=0;
};
class cosine_pdf:public pdf{
  public:
    onb uvw;
    cosine_pdf(const vec3&  w){uvw.build_from_w(w);};
    virtual double value(const vec3& dir)const{   
            auto cos = dot(unit_vector(dir), uvw.w()); 
            return (cos <= 0) ? 0 : cos/pi; 
    }
    virtual vec3 generate()const{
        return uvw.local(random_cosine_direction());
    }
};
class hitable_pdf : public pdf { 
    public: 
        vec3 o;
        hitable* ptr; 
        hitable_pdf(hitable* p, const point3& _o) : ptr(p), o(_o) {} 
 
        virtual double value(const vec3& direction) const{ 
           
            return ptr->pdf_value(o, direction); 
        } 
 
        virtual vec3 generate() const override { 
            
            return ptr->random(o); 
        } 
 
 
};

class mixture_pdf : public pdf { 

    public: 
    pdf* p0;  pdf* p1; 
        mixture_pdf( pdf* _p0, pdf* _p1) { 
            p0 = _p0; p1 =_p1; 
        } 
 
        virtual double value(const vec3& direction) const override { 
            return 0.5 *p0->value(direction) + 0.5 *p1->value(direction); 
        } 
 
        virtual vec3 generate() const override { 
            if (random_double() < 0.5) 
                return p0->generate(); 
            else 
                return p1->generate(); 
        } 
};
#endif