#include <iostream>
#include <cfloat>
#include<chrono>
#include <fstream>
#include <random>
#include "common/vec3.hpp"
#include "common/ray.hpp"
#include "common/random_tool.hpp"
#include "common/material.hpp"
#include "common/sphere.hpp"
#include "common/hitable_list.hpp"
#include "common/camera.hpp"


 random_tool *  _rt_now; 

vec3 color(const ray& r,hitable* scene,int depth){
  auto& rt_now=*_rt_now;
   hit_record rc;
  if(scene->hit(r,0.0,100000.0,rc)){
       ray o_r;
       vec3 atten;
       //using depth to prone branch
       if(depth<50&&rc.mat_ptr->scatter(r,rc,atten,o_r)){
                return atten*color(o_r,scene,depth+1);
       }return vec3(0,0,0);
  }
 // hit nothing ,get sky color
  vec3 dir_norm=unit_vector(r.direction());
  float  t=0.5f*dir_norm.y()+0.5f;
 
  return t*vec3(0.5f,0.7f,1.0f)+(1.0f-t)*vec3(1.0f,1.0f,1.0f);
}
int main() {
  std::cout<<"Beginng"<< std::endl;
//set up random 

 

random_tool rt_now;_rt_now=&rt_now;
 
  //set up scene
  hitable* list[5];
  //main ball
  list[0]=new sphere(vec3(0,0,-1),0.5,new lambertian(vec3(0.1,0.2,0.5),rt_now));
 list[1]=new sphere(vec3(0,-100.5,-1),100,new lambertian(vec3(0.8,0.8,0.0),rt_now));
  list[2]=new sphere(vec3(1,0,-1),0.5,new metal(vec3(0.8,0.6,0.2),0.0,rt_now));
   list[3]=new sphere(vec3(-1,0,-1),0.5,new dielectric(1.5,rt_now));
  list[4]=new sphere(vec3(-1,0,-1),-0.45,new dielectric(1.5,rt_now));
hitable* scene=new hitable_list(list,5);

  std::fstream output("output.ppm", std::ios::in| std::ios::out| std::ios::trunc);
  float aspectRatio=2;
   int ny=100;
  int nx=ny*aspectRatio;

  output<<"P3"<< std::endl;
  output<<nx<<" "<<ny<< std::endl;
  output<<255<< std::endl;
  camera cmr(
    vec3(0.0,0.0,0.0),
    vec3(-1.0*aspectRatio,1.0,-1.0),
    vec3(2.0*aspectRatio,0.0,0.0),
   vec3(0.0,-2.0,0.0) );
  vec3 origin(0.0,0.0,0.0);
 
  int sample_num=1024;
  for(int j=0;j<ny;j++){
        for(int i=0;i<nx;i++){
           vec3 _color(0.0,0.0,0.0);
           //simple oversample Antialiasing
  for(int s=0;s<sample_num;s++){
               float dx=rt_now.rand();  float dy=rt_now.rand();
          float u=(float(i)+dx)/float(nx);
          float v=(float(j)+dy)/float(ny);
          
         auto r=cmr.get_ray(u,v);
        auto color_hit=color(r,scene,0);
            _color+=color_hit;
    }
      _color/=float(sample_num);  

      
  /*               float u=(float(i)+dist(mt_r))/float(nx);
          float v=(float(j)+dist(mt_r))/float(ny);
          
         auto r=cmr.get_ray(u,v);
   
            _color+=color(r,scene); */
            //color correction
           _color=vec3(sqrt( _color.r()),sqrt( _color.g()),sqrt( _color.b()));
          _color*=255.99f;
         output<<int(_color.r())<<" "<<int(_color.g())<<" "<<int(_color.b())<<" ";
  }
    output<<std::endl;
  }
  output.close();
  std::cout<<"Ending"<< std::endl;
}