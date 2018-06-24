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
    vec3 leftdownback;
    vec3 centerpoint;
    float length;
    float width;
    float height;
    
    BoundingBox (){}
    BoundingBox (vec3 ldb, vec3 center, float l, float w, float h) : leftdownback(ldb), centerpoint(center), length(l), width(w), height(h) {}
    
    vec3 getCenter() { return centerpoint; }
    
    int getLongestAxis()
    {
        if(length>width && length>height) return 0;
        else if(width>length && width>height)  return 1;
        else return 2;
    }
    
    float isIntersect(Ray ray) {
        // x axis
        float mint = 1e9;
        bool hit = false;
        float x = leftdownback[0];
        float t = (x-ray.o[0])/ray.d[0];
        float y = ray.o[1] + t * ray.d[1];
        float z = ray.o[2] + t * ray.d[2];
        if (y >= leftdownback[1] && y <= (leftdownback[1]+width) && z >= leftdownback[2] && z <= (leftdownback[2]+height)) {
            if (t < mint) mint = t;
            hit = true;
        }
        
        x = leftdownback[0]+length;
        t = (x-ray.o[0])/ray.d[0];
        y = ray.o[1] + t * ray.d[1];
        z = ray.o[2] + t * ray.d[2];
        if (y >= leftdownback[1] && y <= (leftdownback[1]+width) && z >= leftdownback[2] && z <= (leftdownback[2]+height)) {
            if (t < mint) mint = t;
            hit = true;
        }
        // y axis
        y = leftdownback[1];
        t = (y-ray.o[1])/ray.d[1];
        x = ray.o[0] + t * ray.d[0];
        z = ray.o[2] + t * ray.d[2];
        if (x >= leftdownback[0] && x <= (leftdownback[0]+length) && z >= leftdownback[2] && z <= (leftdownback[2]+height)) {
            if (t < mint) mint = t;
            hit = true;
        }

        y = leftdownback[1]+width;
        t = (y-ray.o[1])/ray.d[1];
        x = ray.o[0] + t * ray.d[0];
        z = ray.o[2] + t * ray.d[2];
        if (x >= leftdownback[0] && x <= (leftdownback[0]+length) && z >= leftdownback[2] && z <= (leftdownback[2]+height)) {
            if (t < mint) mint = t;
            hit = true;
        }
        
        // z axis
        z = leftdownback[2];
        t = (z-ray.o[2])/ray.d[2];
        x = ray.o[0] + t * ray.d[0];
        y = ray.o[1] + t * ray.d[1];
        if (x >= leftdownback[0] && x <= (leftdownback[0]+length) && y >= leftdownback[1] && y <= (leftdownback[1]+width)) {
            if (t < mint) mint = t;
            hit = true;
        }
        
        z = leftdownback[2]+height;
        t = (z-ray.o[2])/ray.d[2];
        x = ray.o[0] + t * ray.d[0];
        y = ray.o[1] + t * ray.d[1];
        if (x >= leftdownback[0] && x <= (leftdownback[0]+length) && y >= leftdownback[1] && y <= (leftdownback[1]+width)) {
            if (t < mint) mint = t;
            hit = true;
        }
        if(hit==true) return mint;
        else return -1;
    }
};


#endif /* BoundingBox_h */
