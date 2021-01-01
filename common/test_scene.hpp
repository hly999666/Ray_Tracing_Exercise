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
#ifndef  TRANSFORM_H
#include "transform.hpp"
#endif

#ifndef  CONSTANT_MEDIUM_H
#include "constant_medium.hpp"
#endif

#ifndef  STB_IMAGE_IMPLEMENTATION
 #define STB_IMAGE_IMPLEMENTATION
 #include "./libs/stb_image.h"
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
      hitable* box1=new translate(
                   new rotate_y(
                   new box(vec3(0,0,0),vec3(165,165,165),white),
                   -18.0),
                   vec3(130,0,65));
     hitable* box2=new translate(
                   new rotate_y(
                   new box(vec3(0,0,0),vec3(165,330,165),white),
                   15.0),
                   vec3(265,0,295));
      list[i++]=new constant_medium(box1,0.01,new constant_texture(vec3(1.0,1.0,1.0)));
      list[i++]=new constant_medium(box2,0.01,new constant_texture(vec3(0.0,0.0,0.0)));

      //light
      list[i++]=new xz_rect(213,343,227,332,554,light);
      vec3 lookfrom(278,278,-800);
      vec3 lookAt(278,278,0);
      double dist_to_focus=10.0;
      double aperture=0.0;
      double vfov=40.0;
      cmr=camera(lookfrom,lookAt,vec3(0,1,0),vfov,nx/ny,aperture,dist_to_focus,0.0,1.0);
      return new hitable_list(list,i);
}

hitable *final_scene_II(camera& cmr,double nx,double ny){
           int nb=20.;
           hitable** list =new hitable*[30];
           hitable** boxlist=new hitable*[10000];
           hitable** boxlist2=new hitable*[10000];
           material* white=new lambertian(new constant_texture(vec3(0.73,0.73,0.73)));
           material* ground=new lambertian(new constant_texture(vec3(0.48,0.83,0.53)));
            //light
            int l=0;
            material* light =new diffuse_light(new constant_texture(vec3(7,7,7)));
           list[l++]=new xz_rect(123,423,147,412,554,light);

           int b=0;
           for(int i=0;i<nb;i++){
                  for(int j=0;j<nb;j++){
                     double w=100;
                     double x0=-1000+i*w;
                     double z0=-1000+j*w;
                     double y0=0;
                     double x1=x0+w;
                     double y1=100*(random_double()+0.01);
                     double z1=z0+w;
                     boxlist[b++]=new box(vec3(x0,y0,z0),vec3(x1,y1,z1),ground);
                 
               }
           }

           //all small box
           list[l++]=new bvh_node(boxlist,b,0,1);
   
           //move sphere
          vec3 center(400,400,200);
          list[l++]=new moving_sphere(center,center+vec3(30,0,0),0,1,50,
                                                         new lambertian(new constant_texture(vec3(0.7,0.3,0.1))));
          list[l++]=new sphere(vec3(260,150,45),50,new dielectric(1.5));
          list[l++]=new sphere(vec3(0,150,145),50,new metal(vec3(0.8,0.8,0.9),10.0));
        
          //subsurface sphere
          hitable* boundary=new sphere(vec3(360,150,145),70,new dielectric(1.5));
           list[l++]=boundary;
           list[l++]=new constant_medium(boundary,0.2,new constant_texture(vec3(0.2,0.4,0.9)));
       
           // all cover mist
            boundary=new sphere(vec3(0,0,0),5000,new dielectric(1.5));
          list[l++]=new constant_medium(boundary,0.0001,new constant_texture(vec3(1.0,1.0,1.0)));
          int n_x,n_y,n_n;
          //unsigned char* tex_map=stbi_load("global_map.jpg",&n_x,&n_y,&n_n,0);
          //material *emat=new lambertian(new image_texture(tex_map,n_x,n_y));
           material * emat = new lambertian(new constant_texture(vec3(0.5, 0.5, 0.3)));
          list[l++]=new sphere(vec3(400,200,400),100,emat);
           
          texture* pertext=new noise_texture(0.1);
          list[l++]=new sphere(vec3(220,280,300),80,new lambertian(pertext));
          int ns=1000;
          for(int j=0;j<ns;j++){
                boxlist2[j]=new sphere(vec3(165*random_double(),165*random_double(),165*random_double()),10,white);
           }
           list[l++]=new translate(
                 new rotate_y(
                     new bvh_node(boxlist2,ns,0.0,1.0),
                        15
                 ),
                 vec3(-100,270,395)
           );  
      //set up camera
           vec3  lookfrom = point3(478, 278, -600);
         vec3   lookat = point3(278, 278, 0);
          float  vfov = 40.0;
    const vec3 vup(0,1,0);
    const auto dist_to_focus = 100.0;
   double  aspect_ratio=1.0;
   double aperture=1.0;
     int image_width=nx;
    int image_height = ny;

    cmr=camera(lookfrom, lookat, vup, vfov, aspect_ratio, aperture, dist_to_focus, 0.0, 1.0);
   return new hitable_list(list,l);
}