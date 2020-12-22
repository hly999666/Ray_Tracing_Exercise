 #ifndef VEC3_H
#include "vec3.hpp"
#endif
 #ifndef RAY_H
#include "ray.hpp"
#endif
 #ifndef GENERAL_HELPER_H
#include "general_helper.hpp"
#endif
 #ifndef TEXTURE_H
#include "texture.hpp"
#endif
#ifndef MATERIAL_H
#include "material.hpp"
#endif
#ifndef  SPHERE_H
#include "sphere.hpp"
#endif
#ifndef  RECT_H
#include "rect.hpp"
#endif
#ifndef  HITABLE_LIST_H
#include "hitable_list.hpp"
#endif
#ifndef  CAMERA_H
#include "camera.hpp"
#endif
#ifndef  BVH_H
#include "bvh.hpp"
#endif
#ifndef  BOX_H
#include "box.hpp"
#endif

hitable* cornell_box(camera& cmr,double nx,double ny){
      hitable** list=new hitable*[8];
      int i=0;
      material* red=new lambertian(new constant_texture(vec3(0.65,0.05,0.05)));
      material* white=new lambertian(new constant_texture(vec3(0.73,0.73,0.73)));
      material* green=new lambertian(new constant_texture(vec3(0.12,0.45,0.15)));
      material* light=new diffuse_light(new constant_texture(vec3(15,15,15)));
      //outer box
      list[i++]=new flip_normals( new yz_rect(0,555,0,555,555,green));
      list[i++]=new yz_rect(0,555,0,555,0,red);
      list[i++]=new flip_normals(new xz_rect(0,555,0,555,555,white));
      list[i++]=new xz_rect(0,555,0,555,0,white);
      list[i++]=new flip_normals(new xy_rect(0,555,0,555,555,white));
 
      //box inside
      list[i++]=new box(vec3(130,0,65),vec3(295,165,230),white);
      list[i++]=new box(vec3(265,0,295),vec3(430,330,460),white);
      
      list[i++]=new xz_rect(213,343,227,332,554,light);
      vec3 lookfrom(278,278,-800);
      vec3 lookAt(278,278,0);
      double dist_to_focus=10.0;
      double aperture=0.0;
      double vfov=40.0;
      cmr=camera(lookfrom,lookAt,vec3(0,1,0),vfov,nx/ny,aperture,dist_to_focus,0.0,1.0);
      return new hitable_list(list,i);
}