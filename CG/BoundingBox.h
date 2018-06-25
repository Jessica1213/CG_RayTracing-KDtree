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
        int index = 0;
        bool hit = false;
        float x = leftdownback[0];
        float t = (x-ray.o[0])/ray.d[0];
        float y = ray.o[1] + t * ray.d[1];
        float z = ray.o[2] + t * ray.d[2];
        if (y >= leftdownback[1] && y <= (leftdownback[1]+width) && z >= leftdownback[2] && z <= (leftdownback[2]+height)) {
            if (t < mint) {
                mint = t;
                index = 1;
            }
            hit = true;
        }
        
        x = leftdownback[0]+length;
        t = (x-ray.o[0])/ray.d[0];
        y = ray.o[1] + t * ray.d[1];
        z = ray.o[2] + t * ray.d[2];
        if (y >= leftdownback[1] && y <= (leftdownback[1]+width) && z >= leftdownback[2] && z <= (leftdownback[2]+height)) {
            if (t < mint) {
                mint = t;
                index = 2;
            }
            hit = true;
        }
        // y axis
        y = leftdownback[1];
        t = (y-ray.o[1])/ray.d[1];
        x = ray.o[0] + t * ray.d[0];
        z = ray.o[2] + t * ray.d[2];
        if (x >= leftdownback[0] && x <= (leftdownback[0]+length) && z >= leftdownback[2] && z <= (leftdownback[2]+height)) {
            if (t < mint) {
                mint = t;
                index = 3;
            }
            hit = true;
        }

        y = leftdownback[1]+width;
        t = (y-ray.o[1])/ray.d[1];
        x = ray.o[0] + t * ray.d[0];
        z = ray.o[2] + t * ray.d[2];
        if (x >= leftdownback[0] && x <= (leftdownback[0]+length) && z >= leftdownback[2] && z <= (leftdownback[2]+height)) {
            if (t < mint) {
                mint = t;
                index = 4;
            }
            hit = true;
        }
        
        // z axis
        z = leftdownback[2];
        t = (z-ray.o[2])/ray.d[2];
        x = ray.o[0] + t * ray.d[0];
        y = ray.o[1] + t * ray.d[1];
        if (x >= leftdownback[0] && x <= (leftdownback[0]+length) && y >= leftdownback[1] && y <= (leftdownback[1]+width)) {
            if (t < mint) {
                mint = t;
                index = 5;
            }
            hit = true;
        }
        
        z = leftdownback[2]+height;
        t = (z-ray.o[2])/ray.d[2];
        x = ray.o[0] + t * ray.d[0];
        y = ray.o[1] + t * ray.d[1];
        if (x >= leftdownback[0] && x <= (leftdownback[0]+length) && y >= leftdownback[1] && y <= (leftdownback[1]+width)) {
            if (t < mint) {
                mint = t;
                index = 6;
            }
            hit = true;
        }
        if(hit==true) {
            switch (index) {
                case 1:
                    x = leftdownback[0];
                    t = (x-ray.o[0])/ray.d[0];
                    y = ray.o[1] + t * ray.d[1];
                    z = ray.o[2] + t * ray.d[2];
                break;
                case 2:
                    x = leftdownback[0]+length;
                    t = (x-ray.o[0])/ray.d[0];
                    y = ray.o[1] + t * ray.d[1];
                    z = ray.o[2] + t * ray.d[2];
                break;
                case 3:
                    y = leftdownback[1];
                    t = (y-ray.o[1])/ray.d[1];
                    x = ray.o[0] + t * ray.d[0];
                    z = ray.o[2] + t * ray.d[2];
                break;
                case 4:
                    y = leftdownback[1]+width;
                    t = (y-ray.o[1])/ray.d[1];
                    x = ray.o[0] + t * ray.d[0];
                    z = ray.o[2] + t * ray.d[2];
                break;
                case 5:
                    z = leftdownback[2];
                    t = (z-ray.o[2])/ray.d[2];
                    x = ray.o[0] + t * ray.d[0];
                    y = ray.o[1] + t * ray.d[1];
                break;
                case 6:
                    z = leftdownback[2]+height;
                    t = (z-ray.o[2])/ray.d[2];
                    x = ray.o[0] + t * ray.d[0];
                    y = ray.o[1] + t * ray.d[1];
                    break;
                default:
                    break;
            }
            vec3 crosspoint(x, y, z);
            return (crosspoint-ray.o).length();
        }
        
        else return -1;
    }
};


#endif /* BoundingBox_h */
