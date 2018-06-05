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

class KDNode {
public:
    BoundingBox bbox;
    KDNode * left;
    KDNode * right;
    vector<Triangle*> triangles;
    
    KDNode() {}
    
    KDNode * build(vector<Triangle*> &tris, int depth) {
        
        if (tris.size() == 0) return NULL;

        KDNode * node  = new KDNode();
        node->triangles = tris;
        node->left = NULL;
        node->right = NULL;
        node->bbox = BoundingBox();
        
        
        if (tris.size() == 1) {
            node->bbox = tris[0]->getBoundingBox();
            node->left = new KDNode();
            node->right = new KDNode();
            node->left->triangles = vector<Triangle*>();
            node->right->triangles = vector<Triangle*>();
            return node;
        }
        
        node->bbox = tris[0]->getBoundingBox();
        
        for(int i=1; i<tris.size(); i++){
            node->bbox.expand(tris[i]->getBoundingBox());
        }
        
        vec3 midpt = vec3(0,0,0);
        

        for (int i=0; i<tris.size(); i++) {
            //find midpoint of all triangles
            midpt = midpt + (tris[i]->get_midpoint() * (1.0/tris.size()));
        }
        vector<Triangle*> left_tris;
        vector<Triangle*> right_tris;
        int axis = node->bbox.longest_axis();
        for (int i=0; i<tris.size(); i++) {
            if (axis==0) {
                if (midpt.x >= tris[i]->get_midpoint().x) {
                    right_tris.push_back(tris[i]);
                }
                else left_tris.push_back(tris[i]);
            }
            
            else if(axis==1) {
                if (midpt.y >= tris[i]->get_midpoint().y) {
                    right_tris.push_back(tris[i]);
                }
                else left_tris.push_back(tris[i]);
            }
            
            else if(axis==2) {
                if (midpt.z >= tris[i]->get_midpoint().z) {
                    right_tris.push_back(tris[i]);
                }
                else left_tris.push_back(tris[i]);
            }
        }
        if (left_tris.size() == 0 && right_tris.size>0) left_tris = right_tris;
        if (right_tris.size() == 0 && left_tris.size>0) right_tris = left_tris;
        
        int matches = 0;
        for (int i=0; i<left_tris.size(); i++) {
            for(int j=0; j<right_tris.size(); j++) {
                if(left_tris[i] == right_tris[j])
                    matches++;
            }
        }
        
        if((float)matches/left_tris.size() < 0.5 && (float)matches/right_tris.size() < 0.5) {
            // recurse down left and right sides
            node->left = build(left_tris, depth+1);
            node->right = build(right_tris, depth+1);
        }
        else {
            node->left = new KDNode();
            node->right  =new KDNode();
            node->left->triangles = vector<Triangle*>();
            node->right->triangles = vector<Triangle*>();
        }
        
        return node;
            
    }
}

#endif /* KDNode_h */
