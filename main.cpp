#include <iostream>
#include <cfloat>
#include <fstream>
#include "common/vec3.hpp"
#include "common/ray.hpp"
#include "common/sphere.hpp"
#include "common/hitable_list.hpp"
 
vec3 color(const ray& r,hitable* scene){
   hit_record rc;
  if(scene->hit(r,0.0,100000.0,rc)){
  
  vec3 n=rc.normal;
    // map to 0-1
    return 0.5*(n+vec3(1.0,1.0,1.0));
  }

  vec3 dir_norm=unit_vector(r.direction());
  float  t=0.5f*dir_norm.y()+0.5f;
 
  return t*vec3(0.5f,0.7f,1.0f)+(1.0f-t)*vec3(1.0f,1.0f,1.0f);
}
int main() {
  std::cout<<"Beginng"<< std::endl;

  //set up scene
  hitable* list[2];
  //main ball
  list[0]=new sphere(vec3(0,0,-1),0.5);
 // extreme large ball, like a ground
 list[1]=new sphere(vec3(0,-100.9,-1),100);
hitable* scene=new hitable_list(list,2);

  std::fstream output("output.ppm", std::ios::in| std::ios::out| std::ios::trunc);
  float aspectRatio=2;
   int ny=100;
  int nx=ny*aspectRatio;

  output<<"P3"<< std::endl;
  output<<nx<<" "<<ny<< std::endl;
  output<<255<< std::endl;
  vec3 origin(0.0,0.0,0.0);
  vec3 top_left(-1.0*aspectRatio,1.0,-1.0);
  vec3 hori(2.0*aspectRatio,0.0,0.0);
  vec3 vert(0.0,-2.0,0.0);
  for(int j=0;j<ny;j++){
        for(int i=0;i<nx;i++){
          float u=float(i)/float(nx);
          float v=float(j)/float(ny);
          vec3 dir=top_left+u*hori+v*vert;
          ray r(origin,dir);
          vec3 _color=color(r,scene);
          _color*=255.99f;
         output<<int(_color.r())<<" "<<int(_color.g())<<" "<<int(_color.b())<<" ";
  }
    output<<std::endl;
  }
  output.close();
  std::cout<<"Ending"<< std::endl;
}