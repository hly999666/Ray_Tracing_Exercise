#include <iostream>
#include <fstream>
#include "common/vec3.hpp"
int main() {
    
  std::cout<<"Beginng"<< std::endl;
  std::fstream output("output.ppm", std::ios::in| std::ios::out| std::ios::trunc);
  vec3 v1(1,2,3);
  int nx=200;
  int ny=100;
  output<<"P3"<< std::endl;
  output<<nx<<" "<<ny<< std::endl;
  output<<255<< std::endl;
  for(int j=0;j<ny;j++){
        for(int i=0;i<nx;i++){
          vec3 v(float(i)/float(nx),1.0f-float(j)/float(ny),0.2f);
          v*=255.99f;
         output<<int(v.r())<<" "<<int(v.g())<<" "<<int(v.b())<<" ";
  }
    output<<std::endl;
  }
  output.close();
  std::cout<<"Ending"<< std::endl;
}