//
//  BoundingBox.h
//  CG
//
//  Created by VIPLAB on 02/05/2018.
//  Copyright © 2018 Jessica. All rights reserved.
//

#include "Ray.h"
#include "algebra3.h"
#ifndef BoundingBox_h
#define BoundingBox_h

class BoundingBox {
public:
    vec3 leftupback;
    vec3 centerpoint;
    float length;
    float width;
    float height;
    
    BoundingBox (vec3 lup, vec3 center, float l, float w, float h) : leftupback(lup), centerpoint(center), length(l), width(w), height(h) {}
    BoundingBox (){}
    
    vec3 getCenter() { return centerpoint; }
    
    int getLongestAxis()
    {
        if(length>width && length>height) return 0;
        else if(width>length && width>height)  return 1;
        else return 2;
    }
//
//    bool hit(Ray ray)
//    {
//        if(ray.d[0] >= min[0] && ray.d[0] <= max[0] && ray.d[1] >= min[1] && ray.d[1] <= max[1]) {
//            return true;
//        }
//        return false;
//    }
};


#endif /* BoundingBox_h */
