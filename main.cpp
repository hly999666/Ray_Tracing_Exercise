#include <iostream>
#include <fstream>
#include "common/vec3.hpp"
#include "common/ray.hpp"
bool hit_shpere(const vec3&center,float radius,const&ray r){
  return false;
}
vec3 color(const ray& r){
  vec3 dir_norm=unit_vector(r.direction());
  float t=0.5f*dir_norm.y()+0.5f;
  if(t<0.1f){
    int a=666;
  }
  return t*vec3(0.5f,0.7f,1.0f)+(1.0f-t)*vec3(1.0f,1.0f,1.0f);
}
int main() {
    
  std::cout<<"Beginng"<< std::endl;
  std::fstream output("output.ppm", std::ios::in| std::ios::out| std::ios::trunc);
  float aspectRatio=2;
  vec3 v1(1,2,3);
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
          vec3 _color=color(r);
          _color*=255.99f;
         output<<int(_color.r())<<" "<<int(_color.g())<<" "<<int(_color.b())<<" ";
  }
    output<<std::endl;
  }
  output.close();
  std::cout<<"Ending"<< std::endl;
}