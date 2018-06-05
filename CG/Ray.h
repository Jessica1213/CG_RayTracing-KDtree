//
//  Ray.h
//  CG
//
//  Created by VIPLAB on 02/05/2018.
//  Copyright © 2018 Jessica. All rights reserved.
//

#ifndef Ray_h
#define Ray_h

class Ray
{
public:
    vec3 o,d;
    Ray(vec3 o, vec3 d) : o(o), d(d) {}
};

#endif /* Ray_h */
