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
    
    BoundingBox (){}
    BoundingBox (vec3 minp, vec3 maxp, vec3 center) : minpoint(minp), maxpoint(maxp), centerpoint(center) {}
    
    vec3 getCenter() { return centerpoint; }
    
    int getLongestAxis()
    {
        float length = maxpoint[0]-minpoint[0];
        float width = maxpoint[1]-minpoint[1];
        float height = maxpoint[2]-minpoint[2];
        if(length>width && length>height) return 0;
        else if(width>length && width>height)  return 1;
        else return 2;
    }
    
    float isIntersect(Ray ray) {
        // x axis
        float mindis = 1e9;
        bool hit = false;
    
        float x = minpoint[0];
        float t = (x-ray.o[0])/ray.d[0];
        float y = ray.o[1] + t * ray.d[1];
        float z = ray.o[2] + t * ray.d[2];
        if (y >= minpoint[1] && y <= maxpoint[1] && z >= minpoint[2] && z <= maxpoint[2]) {
            hit = true;
            vec3 hitpoint(x, y, z);
            float dis = (hitpoint-ray.o).length();
            if (dis < mindis) {
                mindis = dis;
            }
        }
        
        x = maxpoint[0];
        t = (x-ray.o[0])/ray.d[0];
        y = ray.o[1] + t * ray.d[1];
        z = ray.o[2] + t * ray.d[2];
        if (y >= minpoint[1] && y <= maxpoint[1] && z >= minpoint[2] && z <= maxpoint[2]) {
            hit = true;
            vec3 hitpoint(x, y, z);
            float dis = (hitpoint-ray.o).length();
            if (dis < mindis) {
                mindis = dis;
            }
        }
        
        
        // y axis
        y = minpoint[1];
        t = (x-ray.o[1])/ray.d[1];
        x = ray.o[0] + t * ray.d[0];
        z = ray.o[2] + t * ray.d[2];
        if (x >= minpoint[0] && x <= maxpoint[0] && z >= minpoint[2] && z <= maxpoint[2]) {
            hit = true;
            vec3 hitpoint(x, y, z);
            float dis = (hitpoint-ray.o).length();
            if (dis < mindis) {
                mindis = dis;
            }
        }

        y = maxpoint[1];
        t = (x-ray.o[1])/ray.d[1];
        x = ray.o[0] + t * ray.d[0];
        z = ray.o[2] + t * ray.d[2];
        if (x >= minpoint[0] && x <= maxpoint[0] && z >= minpoint[2] && z <= maxpoint[2]) {
            hit = true;
            vec3 hitpoint(x, y, z);
            float dis = (hitpoint-ray.o).length();
            if (dis < mindis) {
                mindis = dis;
            }
        }
        
        // z axis
        z = minpoint[2];
        t = (x-ray.o[2])/ray.d[2];
        x = ray.o[0] + t * ray.d[0];
        y = ray.o[1] + t * ray.d[1];
        if (x >= minpoint[0] && x <= maxpoint[0] && z >= minpoint[1] && z <= maxpoint[1]) {
            hit = true;
            vec3 hitpoint(x, y, z);
            float dis = (hitpoint-ray.o).length();
            if (dis < mindis) {
                mindis = dis;
            }
        }
        
        z = maxpoint[2];
        t = (x-ray.o[2])/ray.d[2];
        x = ray.o[0] + t * ray.d[0];
        y = ray.o[1] + t * ray.d[1];
        if (x >= minpoint[0] && x <= maxpoint[0] && z >= minpoint[1] && z <= maxpoint[1]) {
            hit = true;
            vec3 hitpoint(x, y, z);
            float dis = (hitpoint-ray.o).length();
            if (dis < mindis) {
                mindis = dis;
            }
            
        }
        std::cout << hit << std::endl;
        if(hit) return mindis;
        else return -1;
    }
};


#endif /* BoundingBox_h */
