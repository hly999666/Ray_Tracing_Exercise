#ifndef BVH_H
#define BVH_H
 #ifndef VEC3_H
 #include "vec3.hpp"
#endif
 #ifndef RAY_H
 #include "ray.hpp"
#endif
#ifndef HITABLE_H
#include "hitable.hpp"
#endif

#include <algorithm>
 


class bvh_node:public hitable{
     public:
     hitable* left{nullptr};
     hitable* right{nullptr};
     aabb box;
     bvh_node()=default;
     bvh_node(hitable** l,int n,double time0,double time1);
     virtual bool hit(const ray& r,double tmin,double tmax,hit_record&rc)const;
     virtual bool bounding_box(double t1,double t2,aabb& box)const;
};
bool bvh_node::hit(const ray& r,double tmin,double tmax,hit_record&rc)const{
     if(box.hit(r,tmin,tmax)){
            hit_record left_rc,right_rc;
            bool hit_left=false;bool hit_right=false;
            if(left!=nullptr)hit_left=left->hit(r,tmin,tmax,left_rc);
            if(right!=nullptr)hit_right=right->hit(r,tmin,tmax,right_rc);
            if(hit_left&&hit_right){
                if(left_rc.t<right_rc.t){
                      rc=left_rc;
               }else rc=right_rc;
                    
                    return true;
            }else if(hit_left){
                  rc=left_rc;
                  return true;
            }else if(hit_right){
                  rc=right_rc;
                  return true;
            }else return false;
     }else return false;
};
bool bvh_node::bounding_box(double t1,double t2, aabb& _box)const{
      _box=box;
      return true;
};
 
int box_compare_x(const void* a,const void* b){
    aabb box_l;aabb box_r;
    hitable* ah=*(hitable**)a;
    hitable* bh=*(hitable**)b;
    if(!ah->bounding_box(0.0,0.0,box_l)||!bh->bounding_box(0.0,0.0,box_r)){

          std::cerr<<"no bounding box"<<std::endl;
          return 0;
    }
    if(box_l.min().x()-box_r.min().x()<0.0){
        return -1;
    }else return 1;
}
int box_compare_y(const void* a,const void* b){
    aabb box_l;aabb box_r;
    hitable* ah=*(hitable**)a;
    hitable* bh=*(hitable**)b;
    if(!ah->bounding_box(0.0,0.0,box_l)||!bh->bounding_box(0.0,0.0,box_r)){

          std::cerr<<"no bounding box"<<std::endl;
          return 0;
    }
    if(box_l.min().y()-box_r.min().y()<0.0){
        return -1;
    }else return 1;
}
int box_compare_z(const void* a,const void* b){
    aabb box_l;aabb box_r;
    hitable* ah=*(hitable**)a;
    hitable* bh=*(hitable**)b;
    if(!ah->bounding_box(0.0,0.0,box_l)||!bh->bounding_box(0.0,0.0,box_r)){

          std::cerr<<"no bounding box during compare"<<std::endl;
          return 0;
    }
    if(box_l.min().z()-box_r.min().z()<0.0){
        return -1;
    }else return 1;
}
bvh_node::bvh_node(hitable** l,int n,double time0,double time1){
     int axis=int(3*random_double());
     if(axis==0){
         qsort(l,n,sizeof(hitable*),box_compare_x);
     }else if(axis==1){
         qsort(l,n,sizeof(hitable*),box_compare_y);
     }else {
         qsort(l,n,sizeof(hitable*),box_compare_z);
     }

     if(n==1){
         left=l[0];right=l[0];
     }else if(n==2){
         left=l[0];right=l[1];
     }else{
         int half=n/2;
         left=new bvh_node(l,half,time0,time1);
         right=new bvh_node(l+half,n-half,time0,time1);
     }
     aabb box_left; aabb box_right;
       if(!left->bounding_box(time0,time1,box_left)||!right->bounding_box(time0,time1,box_right)){

          std::cerr<<"no bounding box during construct"<<std::endl;
          
    }else {
        box=aabb_union(box_left,box_right);
    }
};
#endif