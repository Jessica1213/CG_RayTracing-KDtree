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

int const CHILDNUM = 2;
int const DIVIDNUM = 50;
BoundingBox getBoundingBox(std::vector<Triangle> &tri)
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
            minY = MIN(minY, tri[i].points[j][1]);
            minZ = MIN(minZ, tri[i].points[j][2]);
        }
        centerpoint += tri[i].center;
    }
    centerpoint /= tri.size();
    vec3 minp(minX, minY, minZ);
    vec3 maxp(maxX, maxY, maxZ);
    BoundingBox bbox(minp, maxp, centerpoint);
    return bbox;
}

int getLongestAxis(BoundingBox bbox)
{
    float length = bbox.maxpoint[0] - bbox.minpoint[0];
    float width = bbox.maxpoint[1] - bbox.minpoint[1];
    float height = bbox.maxpoint[2] - bbox.minpoint[2];
    if (length > width && length > height) return 0;
    else if(width > length && width > height)  return 1;
    else return 2;
}

class KDNode {
public:
    BoundingBox bbox;
    KDNode * child[2];
    std::vector<Triangle> triangles;
    
    KDNode() {}
    
    KDNode * build(std::vector<Triangle> &tris, int depth) {
        if (tris.size() == 0) return NULL;

        KDNode * node  = new KDNode();
        node->triangles = tris;
        
        node->child[0] = NULL;
        node->child[1] = NULL;
        // get a bounding box surrounding all the triangles
        node->bbox = getBoundingBox(tris);
        int axis = getLongestAxis(node->bbox);
        
        std::cout << depth << " ===== " << node->triangles.size() << " --- " << axis << std::endl;
        if (node->triangles.size() < DIVIDNUM) {
            return node;
        }
        
        std::vector<Triangle> leftTree;
        std::vector<Triangle> rightTree;
        
        vec3 centerpoint = node->bbox.getCenter();
        
        for (int i = 0; i < tris.size(); ++i) {
            if (centerpoint[axis] >= tris[i].center[axis]) {
                leftTree.push_back(tris[i]);
            }
            else if (centerpoint[axis] <= tris[i].center[axis]) {
                rightTree.push_back(tris[i]);
            }
//            for (int j = 0; j < 3; ++j) {
//                if(tris[i].points[j][axis] >= centerpoint[axis]){
//                    rightTree.push_back(tris[i]);
//                    break;
//                }
//            }
//            for (int j = 0; j < 3; ++j) {
//                if(tris[i].points[j][axis] <= centerpoint[axis]){
//                    leftTree.push_back(tris[i]);
//                    break;
//                }
//            }
        }
        
//        if(rightTree.size() == node->triangles.size() || leftTree.size() == node->triangles.size()){
//            return node;
//        }

        // recursive build tree
        node->child[0] = build(leftTree, depth + 1);
        node->child[1] = build(rightTree, depth + 1);

        return node;
            
    }
};

#endif /* KDNode_h */
