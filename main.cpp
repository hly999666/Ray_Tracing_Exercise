#include <iostream>
 
#include<chrono>
#include <fstream>
#include <random>
#include <vector>
#include<thread> 
#include<atomic>  
#include "common/vec3.hpp"
#include "common/ray.hpp"
#include "common/general_helper.hpp"
#include "common/material.hpp"
#include "common/sphere.hpp"
#include "common/hitable_list.hpp"
#include "common/camera.hpp"

const int sample_num=256;
 const  int ny=100;
 const double aspectRatio=2;
 const  int nx=static_cast<int>((double)ny*(double)aspectRatio);

  const int  thread_num =4;
std::atomic_int* _count;

random_tool *  _rt_now; 
std::vector<vec3>* _framebuffer;
hitable* _scene;
camera* _cmr;
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
  double  t=0.5f*dir_norm.y()+0.5;
 
  return t*vec3(0.5,0.7,1.0)+(1.0-t)*vec3(1.0,1.0,1.0);
}


void render_multi_thread(int begin_j,int end_j){
     std::vector<vec3>& framebuffer=*_framebuffer;
    random_tool&rt_now=* _rt_now; 
    std::atomic_int&count=* _count;

   for(int j=begin_j;j<=end_j;j++){
        for(int i=0;i<nx;i++){
           vec3 _color(0.0,0.0,0.0);
           //simple oversample Antialiasing
  for(int s=0;s<sample_num;s++){
          double dx=rt_now.rand();  double dy=rt_now.rand();
          double u=(double(i)+dx)/double(nx);
          double v=(double(j)+dy)/double(ny);
          v=1.0-v;
         auto r=_cmr->get_ray(u,v);
           auto color_hit=color(r,_scene,0);
            _color+=color_hit;
    }
      _color/=double(sample_num);  
 
      _color=vec3(sqrt( _color.r()),sqrt( _color.g()),sqrt( _color.b()));
      _color*=255.999;
     framebuffer[i*ny+j]=_color;
  }
      count++;
      std::cout<<count<<"/"<<ny<<std::endl;

  }


}; 

int main() {
  std::cout<<"Beginng"<< std::endl;
//set up random 
random_tool rt_now;_rt_now=&rt_now;
 
 //set up multi-thread
    std::thread thread_array[thread_num];
   std::atomic_int count{ 0 };
   _count=&count;
//set up framebuffer
std::vector<vec3> framebuffer;framebuffer.resize(nx*ny);
_framebuffer=&framebuffer;
  //set up scene
  hitable* list[5];
  //main ball
  list[0]=new sphere(vec3(0,0,-1),0.5,new lambertian(vec3(0.1,0.2,0.5)));
  list[1]=new sphere(vec3(0,-100.5,-1.0),100,new lambertian(vec3(0.8,0.8,0.0)));
  list[2]=new sphere(vec3(1,0,-1),0.5,new metal(vec3(0.8,0.6,0.2)));
  list[3]=new sphere(vec3(-1,0,-1),0.5,new dielectric(1.5));
  list[4]=new sphere(vec3(-1,0,-1),-0.45,new dielectric(1.5));
  hitable* scene=new hitable_list(list,5);
  _scene=scene;


vec3 lookFrom(3,3,2);
vec3 lookAt(0,0,-1);
double focus_dist=(lookFrom-lookAt).length();
  camera cmr(lookFrom,lookAt,vec3(0,1,0),20.0,aspectRatio,2.0,focus_dist);
 _cmr=&cmr;

  vec3 origin(0.0,0.0,0.0);
 

/*   for(int j=0;j<ny;j++){
        for(int i=0;i<nx;i++){
           vec3 _color(0.0,0.0,0.0);
           //simple oversample Antialiasing
  for(int s=0;s<sample_num;s++){
          double dx=rt_now.rand();  double dy=rt_now.rand();
          double u=(double(i)+dx)/double(nx);
          double v=(double(j)+dy)/double(ny);
          
         auto r=_cmr->get_ray(u,v);
        auto color_hit=color(r,_scene,0);
            _color+=color_hit;
    }
      _color/=double(sample_num);  
 
           _color=vec3(sqrt( _color.r()),sqrt( _color.g()),sqrt( _color.b()));
          _color*=255.99f;
     framebuffer[i*ny+j]=_color;
  }
 
  } */
  //begin rendering multi-thread
    int size=ny/4;
  std::thread tr1(render_multi_thread,0,size-1);
  std::thread tr2(render_multi_thread,size,2*size-1);
  std::thread tr3(render_multi_thread,2*size,3*size-1);
  std::thread tr4(render_multi_thread,3*size,4*size-1);
 tr1.join(); tr2.join();tr3.join();tr4.join();
//output 
  std::fstream output("output.ppm", std::ios::in| std::ios::out| std::ios::trunc);
  output<<"P3"<< std::endl;
  output<<nx<<" "<<ny<< std::endl;
  output<<255<< std::endl;
    for(int j=0;j<ny;j++){
        for(int i=0;i<nx;i++){

        auto& _color= framebuffer[i*ny+j];
        output<<int(_color.r())<<" "<<int(_color.g())<<" "<<int(_color.b())<<" ";
  }
    //output<<std::endl;
  }
  output.close();
  std::cout<<"Ending"<< std::endl;
}