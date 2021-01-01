#ifndef VEC3_H
  #include "vec3.hpp"
#endif
#ifndef GENERAL_HELPER_H  
  #include "general_helper.hpp"
#endif
 
inline double pdf_uniform_on_sphere(const vec3&p){
    return 1/(4*M_PI);
}