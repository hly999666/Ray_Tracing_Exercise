#ifndef HITABLE_LIST_H
#define  HITABLE_LIST_H



#ifndef HITABLE_H
#include "hitable.hpp"
#endif
#ifndef BVH_H
#include "bvh.hpp"
#endif
class aabb;
class hitable_list:public hitable{
public:
hitable **list{nullptr};
int list_size{0};
hitable_list()=default;
hitable_list(hitable** l,int n){list=l;list_size=n;};
virtual bool hit(const ray& r,double t_min,double t_max,hit_record&rc)const;
 virtual bool bounding_box(double t0,double t1,aabb& box)const;
};
  bool hitable_list::hit(const ray& r,double t_min,double t_max,hit_record&rc)const{
   hit_record temp_result;
   bool isHit=false;
   double current_hit_t=t_max;
   for(int i=0;i<list_size;i++){
            if(list[i]->hit(r,t_min,current_hit_t,temp_result)){
              isHit=true;
              if(temp_result.t<current_hit_t){
                current_hit_t=temp_result.t;
                rc=temp_result;
              }
            }
   }

return isHit;
};

   bool hitable_list::bounding_box(double t0,double t1,aabb& box)const{
   return false;
 }

#endif
