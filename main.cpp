#include <iostream>

#include <fstream>

int main() {
    
  std::cout<<"Beginng"<< std::endl;
  std::fstream output("output.ppm", std::ios::in| std::ios::out| std::ios::trunc);
 
  int nx=200;
  int ny=100;
  output<<"P3"<< std::endl;
  output<<nx<<" "<<ny<< std::endl;
  output<<255<< std::endl;
  for(int j=0;j<ny;j++){
        for(int i=0;i<nx;i++){
         float r=float(i)/float(nx);
         float g=1.0f-float(j)/float(ny);
         float b=0.2f;
         output<<int(r*255.99f)<<" "<<int(g*255.99f)<<" "<<int(b*255.99f)<<" ";
  }
    output<<std::endl;
  }
  output.close();
 std::cout<<"Ending"<< std::endl;
}