#include <iostream>
#include "common/general_helper.hpp"
#include "common/pdf.hpp"
inline double pdf_naive(double x){return 3.0*x*x/8.0;}
int main(int argc, char **argv) {
now_rt=new random_tool();
std::cout<<"main_2 !!!"<<std::endl;
   unsigned long long  nums_sample=1;
   double ans=0.0;
   for(  unsigned long long i=1;i<nums_sample+1;i++){
           double x=pow(8.0*random_double(),1.0/3.0);
              std::cout<<"sample "<<x<<std::endl;
           ans+=x*x/pdf_naive(x);
   }

   std::cout<<"Diff "<<ans-8.0/3.0<<std::endl;
  return 0;
}