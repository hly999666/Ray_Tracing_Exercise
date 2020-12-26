#ifndef CONSTANT_MEDIUM_H
#define CONSTANT_MEDIUM_H
 #ifndef  HITABLE_H
#include "hitable.hpp"
#endif
 #ifndef  MATERIAL_H
#include "material.hpp"
#endif
 #ifndef  TEXTURE_H
#include "texture.hpp"
#endif
#include  <cfloat> 
#include <cmath>
class constant_medium : public hitable  {
    public:
         hitable* boundary{nullptr};
         material*  phase_function{nullptr};
        double neg_inv_density{0.0};
        
        constant_medium(hitable* b, double dense, texture *a)
            : boundary(b),
              neg_inv_density(-1/dense),
              phase_function(new isotropic(a)){};
        virtual bool hit(const ray& r, double tmin, double tmax, hit_record& rec)const;
        virtual bool bounding_box(double time0, double time1, aabb& out_b) const{
            return boundary->bounding_box(time0, time1, out_b);
        }
};


bool constant_medium::hit(const ray& r, double tmin, double tmax, hit_record& rec) const {
   /*  const bool enableDebug = false;
    const bool debugging = enableDebug && random_double() < 0.00001; */
    hit_record rec1, rec2;
    //check ray can hit boundary
    if (!boundary->hit(r, HUGE_VAL*-1.0, HUGE_VAL, rec1)) return false;
    //check ray can hit boundary and can get out
    if (!boundary->hit(r, rec1.t+0.0001, HUGE_VAL, rec2))  return false;
/*     if (debugging) std::cerr << "\nt_min=" << rec1.t << ", t_max=" << rec2.t << '\n'; */
    //clamp distance,addressing the case when inside medium
    if (rec1.t <tmin)rec1.t = tmin;
    if (rec2.t >tmax)rec2.t = tmax;
    if (rec1.t >= rec2.t)return false;

    if (rec1.t < 0.0)rec1.t = 0.0;

    const auto ray_length = r.direction().length();
    const auto distance_inside_boundary = (rec2.t - rec1.t) * ray_length;
    
    //random decide ray go through distance
    const auto hit_distance = neg_inv_density * log(random_double());
    // go through distance is large enough,ray go out medium
    if (hit_distance > distance_inside_boundary)return false;

    rec.t = rec1.t + hit_distance / ray_length;
    rec.p = r.point_at_parameter(rec.t);

 /*    if (debugging) {
        std::cerr << "hit_distance = " <<  hit_distance << '\n'
                  << "rec.t = " <<  rec.t << '\n'
                  << "rec.p = " <<rec.p.x()<<" "<< rec.p.y()<<" "<<rec.p.z()<<'\n';
    }
 */
    rec.normal = vec3(1,0,0);  // arbitrary
    rec.mat_ptr = phase_function;

    return true;
}

#endif
