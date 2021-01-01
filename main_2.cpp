#include <iostream>
#include "common/general_helper.hpp"
#include "common/pdf.hpp"
int main(int argc, char **argv) {
now_rt=new random_tool();
std::cout<<"main_2 !!!"<<std::endl;
   unsigned long long  nums_sample=10000000;
   double ans=0.0;
   for(  unsigned long long i=1;i<nums_sample+1;i++){
         
         vec3 d=random_on_unit_sphere();
         double cos_squared=d.z()*d.z();
         ans+=cos_squared/pdf_uniform_on_sphere(d);
         if(i%100000==0){
                std::cout<<"Diff with analytic result "<<ans/(double)i-4.0/3.0*pi<<std::endl;
         }
   }
  return 0;
}