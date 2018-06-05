//
//  Spheres.h
//  CG
//
//  Created by Jessica on 02/05/2018.
//  Copyright © 2018 Jessica. All rights reserved.
//

#include <cmath>
#include "algebra3.h"
#include "Ray.h"
#include "Material.h"
#ifndef Spheres_h
#define Spheres_h

class Sphere
{
private:
    vec3 center;
    double radius;
    
public:
    int material;
    Sphere() {}
    Sphere(vec3 v, double r, int m) : center(v), radius(r), material(m) {}
    
    vec3 getNormal(vec3 cur) { return (cur - center).normalize(); }
    
    vec3 getCenter() { return center; }
    double getRadius() { return radius; }
	bool isInSphere(vec3 point)
	{
		return (point - center).length() < radius;
	}
    float isIntersect(Ray ray)
    {
        vec3 o = ray.o;
        vec3 d = ray.d;
        vec3 oc = o - center;
        double b = 2 * (d*oc);
        double c = oc*oc - radius*radius;
        double disc = b*b - 4 * c;  // b^2-4ac, a is direction distance = 1
        if (disc < 1e-4)
            return -1;
        
        disc = sqrt(disc);
        double t1 = 0.5 * (-b - disc);
        double t2 = 0.5 * (-b + disc);
        if (t1 < 0 && t2 < 0)
            return -1;
        else if (t1 > 0 && t2 < 0)
            return t1 * d.length();
        else if (t1 < 0 && t2 > 0)
            return t2 * d.length();
        else {
            double t = MIN(t1, t2);
            return t * d.length();
        }
        
    }
    
    void expand(Sphere bbox)
    {
        vec3 newcenter = (bbox.center+center)/2;
        double len = MAX((newcenter-center).length()+radius, (newcenter-bbox.center).length()+bbox.radius);
        
        center = newcenter;
        radius = len;
    }
};

#endif /* Spheres_h */
