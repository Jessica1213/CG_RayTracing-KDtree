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
        centerpoint += tri[i].center;
    }
    centerpoint /= tri.size();
    
    for (int i = 0; i < tri.size(); ++i) {
        for (int j = 0; j < 3; ++j) {
            maxX = MAX(maxX, tri[i].points[j][0]);
            maxY = MAX(maxY, tri[i].points[j][1]);
            maxZ = MAX(maxZ, tri[i].points[j][2]);
            minX = MIN(minX, tri[i].points[j][0]);
            minY = MIN(minX, tri[i].points[j][1]);
            minY = MIN(minX, tri[i].points[j][2]);
        }
        
    }
    vec3 leftupback(minX, maxY, minZ);
    float length = maxX-minX;
    float width = maxY-minY;
    float height = maxZ-minZ;
    BoundingBox bbox(leftupback, centerpoint, length, width, height);
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
        
        // get a bounding box surrounding all the triangles
        node->bbox = getBoundingBox(tris);
        if (tris.size() < 100) {
            return node;
        }
        
        vector<Triangle> leftTree;
        vector<Triangle> rightTree;
        
        vec3 centerpoint = node->bbox.getCenter();
        
        int axis = node->bbox.getLongestAxis();
        for (int i=0; i<tris.size(); i++) {
            if (axis==0) {
                if (centerpoint[0] >= tris[i].center[0]) {
                    rightTree.push_back(tris[i]);
                }
                else leftTree.push_back(tris[i]);
            }
            
            else if(axis==1) {
                if (centerpoint[1] >= tris[i].center[1]) {
                    rightTree.push_back(tris[i]);
                }
                else leftTree.push_back(tris[i]);
            }
            
            else if(axis==2) {
                if (centerpoint[2] >= tris[i].center[2]) {
                    rightTree.push_back(tris[i]);
                }
                else leftTree.push_back(tris[i]);
            }
        }
        
//        if (left_tris.size() == 0 && right_tris.size>0) left_tris = right_tris;
//        if (right_tris.size() == 0 && left_tris.size>0) right_tris = left_tris;
        
//        int matches = 0;
//        for (int i=0; i<left_tris.size(); i++) {
//            for(int j=0; j<right_tris.size(); j++) {
//                if(left_tris[i] == right_tris[j])
//                    matches++;
//            }
//        }
        
//        if((float)matches/left_tris.size() < 0.5 && (float)matches/right_tris.size() < 0.5) {
            // recurse down left and right sides
//            node->left = build(left_tris, depth+1);
//            node->right = build(right_tris, depth+1);
//        }
//        else {
//            node->left = new KDNode();
//            node->right  =new KDNode();
//            node->left->triangles = vector<Triangle*>();
//            node->right->triangles = vector<Triangle*>();
//        }

        // recursive build tree
        node->left = build(leftTree, depth + 1);
        node->right = build(rightTree, depth + 1);
        return node;
            
    }
};

#endif /* KDNode_h */
