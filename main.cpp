#include <iostream>
 #include <limits>
#include<chrono>
#include <fstream>
#include <random>
#include <string>
#include <vector>
#include<thread> 
#include<atomic> 
#include "common/vec3.hpp"
#include "common/ray.hpp"
#include "common/general_helper.hpp"
#include "common/texture.hpp"
#include "common/material.hpp"
#include "common/sphere.hpp"
#include "common/rect.hpp"
#include "common/hitable_list.hpp"
#include "common/camera.hpp"
#include "common/bvh.hpp"
#include "common/test_scene.hpp"
#include "common/box.hpp"
#include "common/transform.hpp"

//use opencv to display
#include "opencv2/opencv.hpp"

 //use stb_image to input image
 #define STB_IMAGE_IMPLEMENTATION
 #include "common/libs/stb_image.h"


 const int sample_num=512;
  const double aspectRatio=1;
 const int ny=128;
 const int nx=static_cast<int>((double)ny*(double)aspectRatio);
const int max_depth=50;
    int  thread_num =4;

  const std::string multiThread_mode="scanline";
  std::atomic_int finish_thread{0};
 
  
std::atomic_int* progress_count;

 
std::vector<vec3>* _framebuffer;
hitable* _scene;
camera* _cmr;
color shade(const ray& r,hitable* scene,int depth){
 
   hit_record rc;
   if(depth>max_depth)return color(0,0,0);
   //note t_min should NOT be 0.0,otherwise will let to self-occulsion error due to doubleing-point precision problem
  
  if(scene->hit(r,0.001,inf,rc)){
       ray o_r;
       color atten;
       vec3 emitted=rc.mat_ptr->emitted(rc.u,rc.v,rc.p);
       //using depth to prone branch
       if(depth<max_depth&&rc.mat_ptr->scatter(r,rc,atten,o_r)){
                return emitted+atten*shade(o_r,scene,depth+1);
       }return emitted;
  }
 // hit nothing ,get sky color
 /*  vec3 dir_norm=unit_vector(r.direction());
  double  t=0.5*(dir_norm.y()+1.0);
 
  return (t*color(0.5,0.7,1.0)+(1.0-t)*color(1.0,1.0,1.0))*0.1; */
  return color(0.0,0.0,0.0);
}


void render_multi_thread_scanline(int begin_j,int end_j){
     std::vector<vec3>& framebuffer=*_framebuffer;
  
    std::atomic_int&count=* progress_count;

   for(int j=begin_j;j<=end_j;j++){
        for(int i=0;i<nx;i++){
           vec3 _color(0.0,0.0,0.0);
           //simple oversample Antialiasing
        for(int s=0;s<sample_num;s++){
          double dx=random_double();  double dy=random_double();
          double u=(double(i)+dx)/double(nx-1);
          double v=(double(j)+dy)/double(ny-1);
          v=1.0-v;
         auto r=_cmr->get_ray(u,v);
           auto color_hit=shade(r,_scene,0);
            _color+=color_hit;
    }
     write_color(framebuffer,i*ny+j,_color,sample_num);
   
  }
      count++;
      std::cout<<count<<"/"<<ny<<" pixel row finished"<<std::endl;

  }
finish_thread++;

}; 
void render_multi_thread_progressive(int sampling_target,std::vector<std::vector<vec3>>* frameBufferList,int id){

std::vector<vec3>&  framebuffer_local=frameBufferList->at(id);

 
for(int s=0;s<sampling_target;s++){
   for(int j=0;j<=ny;j++){
        for(int i=0;i<nx;i++){
           vec3 _color(0.0,0.0,0.0);
           //simple oversample Antialiasing
 
          double dx=random_double();  double dy=random_double();
          double u=(double(i)+dx)/double(nx-1);
          double v=(double(j)+dy)/double(ny-1);
          v=1.0-v;
         auto r=_cmr->get_ray(u,v);
         auto color_hit=shade(r,_scene,0);
         framebuffer_local[i*ny+j]+=color_hit;
        }
   }
   
   std::atomic_int&count=* progress_count;
   count++;
  std::cout<<count<<"/"<<sample_num<<" sampling finished"<<std::endl;
}
  
 finish_thread++;
}
hitable *random_scene(camera& _cmr_){
   int n=501;
   hitable ** list=new hitable*[n+1];
   auto tex0=new constant_texture(vec3(0.8,0.8,0.8));
   auto tex1=new constant_texture(vec3(0.5,0.6,0.5));
   list[0]=new sphere(vec3(0,-1000.0,0),1000,new lambertian(new noise_texture(2.0)));

  int	nx,	ny,	nn;
  unsigned	char	*tex_data	=	stbi_load("./uv_checker.jpg",	&nx,	&ny,	&nn,	0);
  material	*mat	=		new	lambertian(new	image_texture(tex_data,	nx,	ny));
/* 
  list[0]=new sphere(vec3(0,-1000.0,0),1000,mat); */
   int count=1;
     for(int i=-11;i<11;i++){
       for(int j=-11;j<11;j++){
                 vec3 center(i+0.9*random_double(),0.2,j+0.9*random_double());
                 //keep main area clear
                 if((center-vec3(4,0.2,0)).length()>0.9){
                                     double mat_rd=random_double();
                                      
                                     if(mat_rd<0.8){
                                       vec3 rand_color(random_double()*random_double(),random_double()*random_double(),random_double()*random_double());
                                       list[count++]=new moving_sphere(
                                         center,center+vec3(0.0,0.5*random_double(),0.0),
                                          0.0,1.0,
                                          0.2, new lambertian(new constant_texture(rand_color))
                                       );

                                     }else if(mat_rd<0.95){
                                           vec3 rand_color(0.5*(1.0+random_double()),0.5*(1.0+random_double()),0.5*(1.0+random_double()));
                                             list[count++]=new sphere(center,0.2,new metal(rand_color,0.5*random_double()));

                                     } else{

                                       list[count++]=new sphere(center,0.2,new dielectric(1.5));
                                     }

                 }

     }

     }

     list[count++]=new sphere(vec3(0,1,0),1.0,new dielectric(1.5));
     list[count++]=new sphere(vec3(-4,1,0),1.0,mat);
     list[count++]=new sphere(vec3(4,1,0),1.0,new metal(vec3(0.7,0.6,0.5),0.0));
     list[count++]=new xy_rect(3,5,1,-3,-2,new diffuse_light(
                                        new constant_texture(
                                          vec3(0.7,0.6,0.5)
                                        )   )  );
                                      
                                     
     
     //camera setting
     vec3 lookFrom(13,2,3);
     vec3 lookAt(0,0,0);
   double focus_dist=10;
   camera cmr(lookFrom,lookAt,vec3(0,1,0),20,aspectRatio,0.1,focus_dist,0.0,1.0);   
   _cmr_=cmr;
     
     //return new hitable_list(list,count);
     return new bvh_node(list,count,0.0,1.0);
};
hitable* simple_scene(camera& _cmr_){
  int n=5;
   hitable ** list=new hitable*[n+1];
  list[0]=new sphere(vec3(0,0,-1),0.5,new lambertian(new constant_texture(vec3(0.1,0.2,0.5))));
  list[1]=new sphere(vec3(0,-100.5,-1.0),100,new lambertian(new constant_texture(vec3(0.8,0.8,0.0))));
  list[2]=new sphere(vec3(1,0,-1),0.5,new metal(vec3(0.8,0.6,0.2)));
  list[3]=new sphere(vec3(-1,0,-1),0.5,new dielectric(1.5));
  list[4]=new sphere(vec3(-1,0,-1),-0.45,new dielectric(1.5)); 



  vec3 lookFrom(0,0,0);
     vec3 lookAt(0,0,-1);
   double focus_dist=1;
   camera cmr(lookFrom,lookAt,vec3(0,1,0),90,aspectRatio,0.0,focus_dist,0.0,0.1);   
   _cmr_=cmr;

   return new hitable_list(list,5);
}
hitable* simple_scene_light(camera& _cmr_){
 texture* pertex=new noise_texture(4);
 hitable **list=new hitable*[4];
 list[0]=new sphere(vec3(0,-1000,0),1000,new lambertian(pertex));
  list[1]=new sphere(vec3(0,2,0),2,new lambertian(pertex));
  //note color of light more than 1,as energy emit
    list[2]=new sphere(vec3(0,7,0),2,new diffuse_light(new constant_texture(vec3(4,4,4))));
   list[3]=new xy_rect(3,5,1,3 ,-2,new diffuse_light(new constant_texture(vec3(4,4,4))));
     
     vec3 lookFrom=vec3(13,2,3)*2.0;
     vec3 lookAt(0,0,0);
   double focus_dist=20;
   camera cmr(lookFrom,lookAt,vec3(0,1,0),20,aspectRatio,0.1,focus_dist,0.0,1.0);   
   _cmr_=cmr;
   return new hitable_list(list,4);
}
int main(int argc, char **argv) {

  std::cout<<"Beginng"<< std::endl;
//set up random 
srand(std::chrono::high_resolution_clock::now().time_since_epoch().count());
now_rt=new random_tool();
// perlin perlin_1();
 //set up multi-thread

std::atomic_int pixel_count{ 0 };
 progress_count=&pixel_count;
//set up framebuffer
std::vector<vec3> framebuffer;
framebuffer.resize(nx*ny);
_framebuffer=&framebuffer;
  //set up scene
  
  camera cmr;
  hitable* scene=cornell_box(cmr,nx,ny);



_scene=scene;
 _cmr=&cmr;


 std::thread tr[4];
 std::vector<std::vector<vec3>>frameBufferList;frameBufferList.resize(4);
 for(auto& framebuffer_local:frameBufferList)framebuffer_local.resize(nx*ny);
  if(multiThread_mode=="scanline"){
  //begin rendering multi-thread
  thread_num=4;
   int size=ny/4;
  tr[0]=std::thread(render_multi_thread_scanline,0,size-1);
  tr[1]=std::thread(render_multi_thread_scanline,size,2*size-1);
  tr[2]=std::thread(render_multi_thread_scanline,2*size,3*size-1);
  tr[3]=std::thread(render_multi_thread_scanline,3*size,4*size-1);
  }else if(multiThread_mode=="progressive"){
    int sampling=sample_num/thread_num;
           
  tr[0]=std::thread(render_multi_thread_progressive,sampling,&frameBufferList,0);
  tr[1]=std::thread(render_multi_thread_progressive,sampling,&frameBufferList,1);
  tr[2]=std::thread(render_multi_thread_progressive,sampling,&frameBufferList,2);
  tr[3]=std::thread(render_multi_thread_progressive,sampling,&frameBufferList,3);
  }
//output 



 
   //simple opencv display

    unsigned char buffer_1[400*500*3];
    int counter=0;
     for(int i=0;i<400;i++){
          
       for(int j=0;j<500;j++){
              buffer_1[counter++]=0;
              buffer_1[counter++]=(double)i/400*255.9;
              buffer_1[counter++]=0;
        }
     }
      int key = 0;

  int pixel_d=1;
  int post_sample=0;
  bool frameBufferUpdate=true;
      while (key != 27) {
            
     if(multiThread_mode=="progressive"){
        pixel_d=*progress_count;
        if(post_sample<pixel_d){
          frameBufferUpdate=true;
          post_sample=pixel_d;
        }else{
          frameBufferUpdate=false;
        }
     }
      if(true){
         if(multiThread_mode=="progressive"){

              framebuffer=reduceFrameBuffer(frameBufferList);

         }
        auto buffer_1=convertFrameBuffer(*_framebuffer,pixel_d);
        cv::Mat image(nx, ny, CV_8UC3,buffer_1.data());
        image=image.t();
        image.convertTo(image, CV_8UC3, 1.0f);
        
        cv::imshow("image", image);
          key = cv::waitKey(10);
         if(finish_thread==thread_num){
     
          std::cout<<"Rendering End"<< std::endl;
          break;
         }
      }
   
    }
  std::fstream output("output.ppm", std::ios::in| std::ios::out| std::ios::trunc);
  output<<"P3"<< std::endl;
  output<<nx<<" "<<ny<< std::endl;
  output<<255<< std::endl;
    if(multiThread_mode=="progressive"){

           framebuffer=reduceFrameBuffer(frameBufferList);
            
    }
    

    for(int j=0;j<ny;j++){
        for(int i=0;i<nx;i++){
        double scale=1.0/pixel_d;
        auto& c= framebuffer[i*ny+j];

    if(multiThread_mode=="progressive"){
           c=convertColor(c,sample_num,0.5);
     } 
        output << static_cast<int>(256 * clamp(c.r(), 0.0, 0.999)) << ' '
                   << static_cast<int>(256 * clamp(c.g(), 0.0, 0.999)) << ' '
                   << static_cast<int>(256 * clamp(c.b(), 0.0, 0.999)) << std::endl;

  }
    //output<<std::endl;
  }
  output.close();
  return 0;
}