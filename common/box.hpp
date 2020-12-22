#ifndef BOX_H
 #define BOX_H

 #ifndef HITABLE_H
 #include "hitable.hpp"
#endif
 #ifndef HITABLE_LIST_H
 #include "hitable_list.hpp"
#endif
 #ifndef VEC3_H
 #include "vec3.hpp"
#endif
 #ifndef RECT_H
 #include "rect.hpp"
#endif
class box:public hitable{
    public:
      vec3 pmin{0.0,0.0,0.0};  vec3 pmax{0.0,0.0,0.0};
      hitable* list_ptr{nullptr};
      box()=default;
      box(const vec3& p0,const vec3& p1,material* ptr);
      virtual bool hit(const ray& r,double t0,double t1,hit_record&rc)const;
      virtual bool bounding_box(double t0,double t1,aabb&box)const{
              box=aabb(pmin,pmax);
              return true;
      };
      
};
box::box(const vec3& p0,const vec3& p1,material* ptr){
      pmin=p0;
      pmax=p1;

      hitable** list=new hitable*[6];
      int count=0;
      list[count++]=new xy_rect(p0.x(),p1.x(),p0.y(),p1.y(),p1.z(),ptr);
      list[count++]=new flip_normals( new xy_rect(p0.x(),p1.x(),p0.y(),p1.y(),p0.z(),ptr));
      list[count++]=new xz_rect(p0.x(),p1.x(),p0.z(),p1.z(),p1.y(),ptr);
      list[count++]=new flip_normals( new xz_rect(p0.x(),p1.x(),p0.z(),p1.z(),p0.y(),ptr));
      list[count++]=new yz_rect(p0.y(),p1.y(),p0.z(),p1.z(),p1.x(),ptr);
      list[count++]=new flip_normals( new yz_rect(p0.y(),p1.y(),p0.z(),p1.z(),p0.x(),ptr));
      list_ptr=new hitable_list(list,6);
};
bool box::hit(const ray& r,double t0,double t1,hit_record& rc)const{
     return list_ptr->hit(r,t0,t1,rc);
 };
#endif