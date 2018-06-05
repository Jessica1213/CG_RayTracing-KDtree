//
//  BoundingBox.h
//  CG
//
//  Created by VIPLAB on 02/05/2018.
//  Copyright © 2018 Jessica. All rights reserved.
//

#include "Ray.h"
#ifndef BoundingBox_h
#define BoundingBox_h

class BoundingBox {
public:
    vec3 leftupback;
    float length;
    float width;
    float height;
    
    BoundingBox (vec3 lup, float l, float w, float h) : leftupback(lup), length(l), width(w), height(h) {}
    BoundingBox (){}
    
//    void expand(BoundingBox bbox)
//    {
//        for (int i = 0; i < 3; ++i) {
//            if (bbox.max[i] > max[i]) max[i] = bbox.max[i];
//            if (bbox.min[i] < min[i]) min[i] = bbox.min[i];
//        }
//        if (bbox.max[0] > max[0]) {
//            for (int i = 0; i < 3; ++i) max[i] = bbox.max[0];
//        }
//        if (bbox.min[0] < min[0]) {
//            for (int i = 0; i < 3; ++i) min[i] = bbox.min[0];
//        }
//    }
    
//    int getLongestAxis()
//    {
//        float x = max[0] - min[0];
//        float y = max[0] - min[0];
//        float z = max[0] - min[0];
//        
//        if (x > y && x > z) return 0;
//        else if ( y > x && y > z) return 1;
//        else return 2;
//    }
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
