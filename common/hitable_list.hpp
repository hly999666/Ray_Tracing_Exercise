#ifndef HITABLE_LIST_H
#define  HITABLE_LIST_H



#ifndef HITABLE_H
#include "hitable.hpp"
#endif

class hitable_list:public hitable{
public:
hitable **list{nullptr};
int list_size{0};
hitable_list()=default;
hitable_list(hitable** l,int n){list=l;list_size=n;};
virtual bool hit(const ray& r,float t_min,float t_max,hit_record&rc)const;

};
  bool hitable_list::hit(const ray& r,float t_min,float t_max,hit_record&rc)const{


return false;
};



#endif
