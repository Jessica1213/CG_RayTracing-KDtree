//
//  Triangles.h
//  CG
//
//  Created by Jessica on 02/05/2018.
//  Copyright © 2018 Jessica. All rights reserved.
//

#include "Ray.h"
#include "Material.h"
#include <algorithm>
#include "BoundingBox.h"
#include "algebra3.h"
#ifndef Triangles_h
#define Triangles_h

float determinant(mat3 m)
{
    return m[0][0] * m[1][1] * m[2][2] +
    m[0][1] * m[1][2] * m[2][0] +
    m[0][2] * m[1][0] * m[2][1] -
    m[0][2] * m[1][1] * m[2][0] -
    m[0][1] * m[1][0] * m[2][2] -
    m[0][0] * m[1][2] * m[2][1];
}

class Triangle {
private:
    
public:
    vec3 points[3];
	vec3 normal;
	vec3 center;
	int material;
    Triangle(vec3 points[], vec3 nor, vec3 cen, int m) :  normal(nor), center(cen), material(m)
    {
        for (int i=0; i<3; ++i)
            this->points[i] = points[i];
    }
    
	Triangle() {}
    float isIntersect(Ray ray)
    {
        vec3 v0 = points[0];
        vec3 v1 = points[1]-points[0];
        vec3 v2 = points[2]-points[0];
        
        vec3 o = ray.o;
        vec3 d = ray.d;
        mat3 matrix(-d, v1, v2);
        matrix = matrix.transpose();
        if( determinant(matrix) == 0) return -1;
        vec3 sol = matrix.inverse() * (o-v0);
        if (sol[0] > 0 && sol[1] + sol[2] <= 1 && sol[1] >= 0 && sol[2] >= 0 && sol[1] <= 1 && sol[2] <= 1) {
            return sol[0] * d.length();
        }
        else
            return -1;
    }
    
    BoundingBox getBoundingBox()
    {
        float minX, maxX, minY, maxY, minZ, maxZ;
        maxX = MAX(points[0][0], MAX(points[1][0], points[2][0]));
        minX = MIN(points[0][0], MIN(points[1][0], points[2][0]));
        maxY = MAX(points[0][1], MAX(points[1][1], points[2][1]));
        minY = MIN(points[0][1], MIN(points[1][1], points[2][1]));
        maxZ = MAX(points[0][2], MAX(points[1][2], points[2][2]));
        minZ = MIN(points[0][2], MIN(points[1][2], points[2][2]));
            
        vec3 leftupback(minX, maxY, minZ);
        float length = maxX-minX;
        float width = maxY-minY;
        float height = maxZ-minZ;
        BoundingBox bbox(leftupback, length, width, height);
        return bbox;
        
    }

};

#endif /* Triangles_h */
