#include <iostream>
 #include <limits>
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
#include "common/bvh.hpp"
#include "opencv2/opencv.hpp"

 
  int sample_num=1024;
   double aspectRatio=2;
  int ny=512;
  int nx=static_cast<int>((double)ny*(double)aspectRatio);
int max_depth=50;
  const int  thread_num =4;
std::atomic_int* _count;

 
std::vector<vec3>* _framebuffer;
hitable* _scene;
camera* _cmr;
color shade(const ray& r,hitable* scene,int depth){
 
   hit_record rc;
   if(depth>max_depth)return color(0,0,0);
   //note t_min should NOT be 0.0,eitherwise will produce some unexplainable numerical problem
  if(scene->hit(r,0.001,inf,rc)){
       ray o_r;
       color atten;
       //using depth to prone branch
       if(depth<max_depth&&rc.mat_ptr->scatter(r,rc,atten,o_r)){
                return atten*shade(o_r,scene,depth+1);
       }return color(0,0,0);
  }
 // hit nothing ,get sky color
  vec3 dir_norm=unit_vector(r.direction());
  double  t=0.5*(dir_norm.y()+1.0);
 
  return t*color(0.5,0.7,1.0)+(1.0-t)*color(1.0,1.0,1.0);
}


void render_multi_thread(int begin_j,int end_j){
     std::vector<vec3>& framebuffer=*_framebuffer;
  
    std::atomic_int&count=* _count;

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
      std::cout<<count<<"/"<<ny<<std::endl;

  }


}; 
hitable *random_scene(camera& _cmr_){
   int n=500;
   hitable ** list=new hitable*[n+1];
   list[0]=new sphere(vec3(0,-1000.0,0),1000,new lambertian(vec3(0.5,0.5,0.5)));
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
                                          0.2, new lambertian(rand_color)
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
     list[count++]=new sphere(vec3(-4,1,0),1.0,new lambertian(vec3(0.4,0.2,0.1)));
     list[count++]=new sphere(vec3(4,1,0),1.0,new metal(vec3(0.7,0.6,0.5),0.0));
     
     
     //camera setting
     vec3 lookFrom(13,2,3);
     vec3 lookAt(0,0,0);
   double focus_dist=10;
   camera cmr(lookFrom,lookAt,vec3(0,1,0),20,aspectRatio,0.1,focus_dist,0.0,1.0);   
   _cmr_=cmr;
     
     return new hitable_list(list,count);
};
hitable* simple_scene(camera& _cmr_){
  int n=5;
   hitable ** list=new hitable*[n+1];
  list[0]=new sphere(vec3(0,0,-1),0.5,new lambertian(vec3(0.1,0.2,0.5)));
  list[1]=new sphere(vec3(0,-100.5,-1.0),100,new lambertian(vec3(0.8,0.8,0.0)));
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
int main(int argc, char **argv) {
  std::cout<<"Beginng"<< std::endl;
//set up random 
 
now_rt=new random_tool();
 aspectRatio=2;
 nx=256;
 ny=static_cast<int>(nx/aspectRatio);
 sample_num=32;

 //set up multi-thread
    std::thread thread_array[thread_num];
   std::atomic_int count{ 0 };
   _count=&count;
//set up framebuffer
std::vector<vec3> framebuffer;framebuffer.resize(nx*ny);
_framebuffer=&framebuffer;
  //set up scene
/*   */
  camera cmr;
  hitable* scene=random_scene(cmr);



  _scene=scene;
 _cmr=&cmr;
 
  //begin rendering multi-thread
    int size=ny/4;
  std::thread tr1(render_multi_thread,0,size-1);
  std::thread tr2(render_multi_thread,size,2*size-1);
  std::thread tr3(render_multi_thread,2*size,3*size-1);
  std::thread tr4(render_multi_thread,3*size,4*size-1);

//output 
  std::fstream output("output.ppm", std::ios::in| std::ios::out| std::ios::trunc);
  output<<"P3"<< std::endl;
  output<<nx<<" "<<ny<< std::endl;
  output<<255<< std::endl;
    for(int j=0;j<ny;j++){
        for(int i=0;i<nx;i++){

        auto& c= framebuffer[i*ny+j];
        output << static_cast<int>(256 * clamp(c.r(), 0.0, 0.999)) << ' '
                   << static_cast<int>(256 * clamp(c.g(), 0.0, 0.999)) << ' '
                   << static_cast<int>(256 * clamp(c.b(), 0.0, 0.999)) << std::endl;

  }
    //output<<std::endl;
  }
  output.close();


  
 
  

std::cout<<"Rendering End"<< std::endl;
   //simple opencv display

    unsigned char buffer_1[400*500*3];
    int counter=0;
     for(int i=0;i<400;i++){
          
       for(int j=0;j<500;j++){
              buffer_1[counter++]=0;
              buffer_1[counter++]=(float)i/400*255.9;
              buffer_1[counter++]=0;
     }
     }
      int key = 0;
      while (key != 27) {
       auto buffer_1=convertFrameBuffer(*_framebuffer);
        cv::Mat image(nx, ny, CV_8UC3,buffer_1.data());
        image=image.t();
        image.convertTo(image, CV_8UC3, 1.0f);
        
        cv::imshow("image", image);
        key = cv::waitKey(10);
      
    }
    tr1.join(); tr2.join();tr3.join();tr4.join(); 
    
  return 0;
}