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
    vec3 minpoint;
    vec3 maxpoint;
    vec3 centerpoint;
    int longestAxis;
    
    BoundingBox (){}
    BoundingBox (vec3 minp, vec3 maxp, vec3 center) : minpoint(minp), maxpoint(maxp), centerpoint(center) {
        float length = maxpoint[0] - minpoint[0];
        float width = maxpoint[1] - minpoint[1];
        float height = maxpoint[2] - minpoint[2];
        if (length > width && length > height) longestAxis = 0;
        else if(width > length && width > height) longestAxis = 1;
        else longestAxis = 2;
    }
    
    vec3 getCenter() { return centerpoint; }
    
    float isIntersect(Ray ray) {
        // x axis
        float mindis = 1e9;
        bool hit = false;
    
        for (int i = 0; i < 3; i++) {
            float t = (minpoint[i]-ray.o[i])/ray.d[i];
            float p1 = ray.o[(i + 1) % 3] + t * ray.d[(i + 1) % 3];
            float p2 = ray.o[(i + 2) % 3] + t * ray.d[(i + 2) % 3];
            
            if (p1 >= minpoint[(i + 1) % 3] && p1 <= maxpoint[(i + 1) % 3] && p2 >= minpoint[(i + 2) % 3] && p2 <= maxpoint[(i + 2) % 3])
            {
                hit = true;
                float dis = sqrt((t * ray.d[i])*(t * ray.d[i])+
                                 (t * ray.d[(i+1)%3])*(t * ray.d[(i+1)%3])+
                                 (t * ray.d[(i+2)%3])*(t * ray.d[(i+1)%3]));
                if (dis < mindis) {
                    mindis = dis;
                }
            }
        }
        
        for (int i = 0; i < 3; i++) {
            float t = (maxpoint[i]-ray.o[i])/ray.d[i];
            float p1 = ray.o[(i + 1) % 3] + t * ray.d[(i + 1) % 3];
            float p2 = ray.o[(i + 2) % 3] + t * ray.d[(i + 2) % 3];
            
            if (p1 >= minpoint[(i + 1) % 3] && p1 <= maxpoint[(i + 1) % 3] && p2 >= minpoint[(i + 2) % 3] && p2 <= maxpoint[(i + 2) % 3])
            {
                hit = true;
                float dis = sqrt((t * ray.d[i])*(t * ray.d[i])+
                                 (t * ray.d[(i+1)%3])*(t * ray.d[(i+1)%3])+
                                 (t * ray.d[(i+2)%3])*(t * ray.d[(i+1)%3]));
                if (dis < mindis) {
                    mindis = dis;
                }
            }
        }
        
        if(hit) return mindis;
        else return -1;
    }
};


#endif /* BoundingBox_h */
