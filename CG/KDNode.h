//
//  KDNode.h
//  CG
//
//  Created by VIPLAB on 29/05/2018.
//  Copyright © 2018 Jessica. All rights reserved.
//

#include <vector>
#include <cmath>
#include "Triangles.h"
#include "BoundingBox.h"
#ifndef KDNode_h
#define KDNode_h

BoundingBox getBoundingBox(vector<Triangle> &tri)
{
    vec3 centerpoint(0, 0, 0);
    float minX = 1e9, maxX = 1e-9;
    float minY = 1e9, maxY = 1e-9;
    float minZ = 1e9, maxZ = 1e-9;
    for (int i = 0; i < tri.size(); ++i) {
        for (int j = 0; j < 3; ++j) {
            maxX = MAX(maxX, tri[i].points[j][0]);
            maxY = MAX(maxY, tri[i].points[j][1]);
            maxZ = MAX(maxZ, tri[i].points[j][2]);
            minX = MIN(minX, tri[i].points[j][0]);
            minY = MIN(minX, tri[i].points[j][1]);
            minY = MIN(minX, tri[i].points[j][2]);
        }
        centerpoint += tri[i].center;
    }
    centerpoint /= tri.size();
    
    vec3 leftdownback(minX, minY, minZ);
    float length = maxX-minX;
    float width = maxY-minY;
    float height = maxZ-minZ;
    BoundingBox bbox(leftdownback, centerpoint, length, width, height);
    return bbox;
}


class KDNode {
public:
    BoundingBox bbox;
    KDNode * left;
    KDNode * right;
    vector<Triangle> triangles;
    
    KDNode() {}
    
    KDNode * build(vector<Triangle> &tris, int depth) {
        if (tris.size() == 0) return NULL;

        KDNode * node  = new KDNode();
        node->triangles = tris;
        node->left = NULL;
        node->right = NULL;
        std::cout << depth << " ===== " << node->triangles.size() << std::endl;
        // get a bounding box surrounding all the triangles
        node->bbox = getBoundingBox(tris);

        if (node->triangles.size() < 50) {
            std::cout << node->triangles.size() << std::endl;
            return node;
        }
        
        vector<Triangle> leftTree;
        vector<Triangle> rightTree;
        
        vec3 centerpoint = node->bbox.getCenter();
        
        int axis = node->bbox.getLongestAxis();
        for (int i=0; i<tris.size(); i++) {
            if (centerpoint[axis] >= tris[i].center[axis]) {
                leftTree.push_back(tris[i]);
            }
            else rightTree.push_back(tris[i]);
        }

        // recursive build tree
        node->left = build(leftTree, depth + 1);
        node->right = build(rightTree, depth + 1);
        return node;
            
    }
};

#endif /* KDNode_h */
