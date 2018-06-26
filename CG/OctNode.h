//
//  OctNode.h
//  CG
//
//  Created by Jessica on 02/05/2018.
//  Copyright © 2018 Jessica. All rights reserved.
//

#include <vector>
#include <cmath>
#include "BoundingBox.h"
#include "Triangles.h"
#include "KDnode.h"

#ifndef OctNode_h
#define OctNode_h

int const OCTDIRECTION = 8;

Sphere getBoundingSphere(const std::vector<Triangle> &tri)
{
	vec3 centerpoint(0, 0, 0);
	for (int i = 0; i < tri.size(); ++i) {
		centerpoint += tri[i].center;
	}
	centerpoint /= tri.size();

	float max = 1e-9;
	for (int i = 0; i < tri.size(); ++i) {
		for (int j = 0; j < 3; ++j) {
			float cur = (tri[i].points[j] - centerpoint).length();
			if (cur > max)
				max = cur;
		}
	}

	return Sphere(centerpoint, max, 0);
}

class OctNode {
public:
    BoundingBox bbox;
	OctNode * eightDirection[8];
    std::vector<Triangle> triangles;
    
    OctNode() {}
    
    OctNode * build(std::vector<Triangle> &tris, int depth)
    {
		//cout << "depth: " << depth << endl;
		if (tris.size() == 0) return NULL;

        OctNode * node = new OctNode();
        node->triangles = tris;
        
        for (int i = 0; i < OCTDIRECTION; i++) {
            node->eightDirection[i] = NULL;
        }
        
		// get a bounding box surrounding all the triangles
        node->bbox = getBoundingBox(tris);

		if (tris.size() < 100) {
			return node;
		}
		
		std::vector<std::vector<Triangle>> alldirectionTree(OCTDIRECTION);
        
        vec3 centerpoint = node->bbox.getCenter();

		//// up left front
		//for (int i = 0; i < tris.size(); ++i) {
		//	for (int j = 0; j < 3; j++) {
		//		if (tris[i].points[j][1] >= centerpoint[1] && tris[i].points[j][0] <= centerpoint[1] && tris[i].points[j][2] >= centerpoint[2]) {
		//			alldirectionTree[0].push_back(tris[i]);
		//			break;
		//		}
		//	}
		//}

		//// up left back
		//for (int i = 0; i < tris.size(); ++i) {
		//	for (int j = 0; j < 3; j++) {
		//		if (tris[i].points[j][1] >= centerpoint[1] && tris[i].points[j][0] <= centerpoint[0] && tris[i].points[j][2] <= centerpoint[2]) { 
		//			alldirectionTree[1].push_back(tris[i]);
		//			break;
		//		}
		//	}
		//}

		//// up right front
		//for (int i = 0; i < tris.size(); ++i) {
		//	for (int j = 0; j < 3; j++) {
		//		if (tris[i].points[j][1] >= centerpoint[1] && tris[i].points[j][0] >= centerpoint[0] && tris[i].points[j][2] >= centerpoint[2]) { 
		//			alldirectionTree[2].push_back(tris[i]);
		//			break;
		//		}
		//	}
		//}
		//
		//// up right back
		//for (int i = 0; i < tris.size(); ++i) {
		//	for (int j = 0; j < 3; j++) {
		//		if (tris[i].points[j][1] >= centerpoint[1] && tris[i].points[j][0] >= centerpoint[0] && tris[i].points[j][2] <= centerpoint[2]) { 
		//			alldirectionTree[3].push_back(tris[i]);
		//			break;
		//		}
		//	}
		//}

		//// down left front
		//for (int i = 0; i < tris.size(); ++i) {
		//	for (int j = 0; j < 3; j++) {
		//		if (tris[i].points[j][1] <= centerpoint[1] && tris[i].points[j][0] <= centerpoint[0] && tris[i].points[j][2] >= centerpoint[2]) { 
		//			alldirectionTree[4].push_back(tris[i]);
		//			break;
		//		}
		//	}
		//}

		//// down left back
		//for (int i = 0; i < tris.size(); ++i) {
		//	for (int j = 0; j < 3; j++) {
		//		if (tris[i].points[j][1] <= centerpoint[1] && tris[i].points[j][0] <= centerpoint[0] && tris[i].points[j][2] <= centerpoint[2]) { 
		//			alldirectionTree[5].push_back(tris[i]);
		//			break;
		//		}
		//	}
		//}

		//// down right front
		//for (int i = 0; i < tris.size(); ++i) {
		//	for (int j = 0; j < 3; j++) {
		//		if (tris[i].points[j][1] <= centerpoint[1] && tris[i].points[j][0] >= centerpoint[0] && tris[i].points[j][2] >= centerpoint[2]) { 
		//			alldirectionTree[6].push_back(tris[i]);
		//			break;
		//		}
		//	}
		//}

		//// down right back
		//for (int i = 0; i < tris.size(); ++i) {
		//	for (int j = 0; j < 3; j++) {
		//		if (tris[i].points[j][1] <= centerpoint[1] && tris[i].points[j][0] >= centerpoint[0] && tris[i].points[j][2] <= centerpoint[2]) { 
		//			alldirectionTree[7].push_back(tris[i]);
		//			break;
		//		}
		//	}
		//}

		// up left front
		for (int i = 0; i < tris.size(); ++i) {
			for (int j = 0; j < 3; j++) {
				if (tris[i].points[j][0] >= centerpoint[0] && tris[i].points[j][1] <= centerpoint[1] && tris[i].points[j][2] >= centerpoint[2]) {
					alldirectionTree[0].push_back(tris[i]);
					break;
				}
			}
		}

		// up left back
		for (int i = 0; i < tris.size(); ++i) {
			for (int j = 0; j < 3; j++) {
				if (tris[i].points[j][0] >= centerpoint[0] && tris[i].points[j][1] <= centerpoint[1] && tris[i].points[j][2] <= centerpoint[2]) {
					alldirectionTree[1].push_back(tris[i]);
					break;
				}
			}
		}

		// up right front
		for (int i = 0; i < tris.size(); ++i) {
			for (int j = 0; j < 3; j++) {
				if (tris[i].points[j][0] >= centerpoint[0] && tris[i].points[j][1] >= centerpoint[1] && tris[i].points[j][2] >= centerpoint[2]) {
					alldirectionTree[2].push_back(tris[i]);
					break;
				}
			}
		}

		// up right back
		for (int i = 0; i < tris.size(); ++i) {
			for (int j = 0; j < 3; j++) {
				if (tris[i].points[j][0] >= centerpoint[0] && tris[i].points[j][1] >= centerpoint[1] && tris[i].points[j][2] <= centerpoint[2]) {
					alldirectionTree[3].push_back(tris[i]);
					break;
				}
			}
		}

		// down left front
		for (int i = 0; i < tris.size(); ++i) {
			for (int j = 0; j < 3; j++) {
				if (tris[i].points[j][0] <= centerpoint[0] && tris[i].points[j][1] <= centerpoint[1] && tris[i].points[j][2] >= centerpoint[2]) {
					alldirectionTree[4].push_back(tris[i]);
					break;
				}
			}
		}

		// down left back
		for (int i = 0; i < tris.size(); ++i) {
			for (int j = 0; j < 3; j++) {
				if (tris[i].points[j][0] <= centerpoint[0] && tris[i].points[j][1] <= centerpoint[1] && tris[i].points[j][2] <= centerpoint[2]) {
					alldirectionTree[5].push_back(tris[i]);
					break;
				}
			}
		}

		// down right front
		for (int i = 0; i < tris.size(); ++i) {
			for (int j = 0; j < 3; j++) {
				if (tris[i].points[j][0] <= centerpoint[0] && tris[i].points[j][1] >= centerpoint[1] && tris[i].points[j][2] >= centerpoint[2]) {
					alldirectionTree[6].push_back(tris[i]);
					break;
				}
			}
		}

		// down right back
		for (int i = 0; i < tris.size(); ++i) {
			for (int j = 0; j < 3; j++) {
				if (tris[i].points[j][0] <= centerpoint[0] && tris[i].points[j][1] >= centerpoint[1] && tris[i].points[j][2] <= centerpoint[2]) {
					alldirectionTree[7].push_back(tris[i]);
					break;
				}
			}
		}

        // recursive build tree
		for (int i = 0; i < OCTDIRECTION; ++i) {
			node->eightDirection[i] = build(alldirectionTree[i], depth + 1);
		}
        
        return node;
    }
    
};

#endif /* OctNode_h */
