//
//  Material.h
//  CG
//
//  Created by Jessica on 02/05/2018.
//  Copyright © 2018 Jessica. All rights reserved.
//

#ifndef Material_h
#define Material_h

class Material
{
public:
    vec3 rgb;
    vec3 coef; //Ka, Kd, Ks are the coefficients of the ambient, diffuse, and specular
    double exp, reflect, refract, nr;
    
    Material() {}
    Material(vec3 color, vec3 cof, double e, double r1, double r2, double n) : rgb(color), coef(cof), exp(e), reflect(r1), refract(r2), nr(n) {}
};

#endif /* Material_h */
