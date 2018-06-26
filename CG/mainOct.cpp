//
//  main.cpp
//  CG_HW2
//
//  Created by Jessica on 4/8/18.
//  Copyright © 2018 Jessica. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include "algebra3.h"
#include "imageIO.h"
#include "Ray.h"
#include "Material.h"
#include "Spheres.h"
#include "Triangles.h"
#include "OctNode.h"
#include <time.h>
#include <stdlib.h>
using namespace std;

void ClampColor(vec3 &color)
{
    for (int i = 0; i < 3; ++i)
        color[i] = MAX(MIN(color[i], 1.0f), 0.0f);
}

Triangle * hit(OctNode*node, Ray &ray) {
	if (node == NULL)
		return NULL;

	float dis = node->bbox.isIntersect(ray);
	if (dis > 0) {
		vector<pair <float, int>> hitpair;
		for (int i = 0; i < OCTDIRECTION; i++) {
			if (node->eightDirection[i] != NULL) {
				float dist = node->eightDirection[i]->bbox.isIntersect(ray);
				if (dist > 0) {
					hitpair.push_back(make_pair(dist, i));
				}
			}
		}
		// leaf node
		if (hitpair.size() == 0) {
			float distance = 1e9;
			Triangle * nearestTriangle = NULL;
			for (int j = 0; j < node->triangles.size(); j++) {
				float dist = node->triangles[j].isIntersect(ray);
				if (dist > 0 && dist < distance) {
					nearestTriangle = &(node->triangles[j]);
					distance = dist;
				}
			}
			return nearestTriangle;
		}
		else {
			sort(begin(hitpair), end(hitpair));

			for (int i = 0; i < hitpair.size(); i++) {
				int index = hitpair[i].second;
				Triangle * result = hit(node->eightDirection[index], ray);
				if (result) {
					return result;
				}
			}
		}
	}

	return NULL;
}

vec3 RayTracing(OctNode *node, Ray &ray, vector<Material> &materials, vec3 &lightpos, vec3 &lightColor, int depth)
{
    vec3 backgroundColor(0,0,0);
    if (depth == 0) return backgroundColor;
    
    vec3 color(0, 0, 0);    
    vec3 intersectPoint, normal;
    int materialindex = 0;

	Triangle * nearestTriangle = hit(node, ray);
	// No intersect
	if (nearestTriangle == NULL) {
		return backgroundColor;
	}
	// intersect with triangle
	else {
		float dist = nearestTriangle->isIntersect(ray);
		if (dist < 0)
			cout << "ERROR!!!!!!!!!!!!!!!!" << endl;
		
		intersectPoint = ray.o + ray.d * dist;
		normal = nearestTriangle->normal;
		materialindex = nearestTriangle->material;
	}
  		
	Ray towardLight(intersectPoint + normal*1e-5, (lightpos - intersectPoint).normalize());
    
    Triangle * block = hit(node, towardLight);
	Material material = materials[materialindex];
    
    color += material.coef[0] * lightColor;
    
    if (block==NULL) {
        // diffuse component  => kd*(li* (N dot L))
        color += material.coef[1] * lightColor * (normal * towardLight.d);
        // specular component  => ks*(li*((N dot H) ^ exp), H=L+V
        vec3 h = (towardLight.d - ray.d).normalize();
        double exp = material.exp;
        color += material.coef[2] * lightColor * pow((normal*h), exp);
    }


	color[0] *= material.rgb[0];
	color[1] *= material.rgb[1];
	color[2] *= material.rgb[2];

	ClampColor(color);

	// ===== reflect =====
	vec3 reflectDirection = (ray.d - 2 * (ray.d* normal) * normal).normalize();
	Ray reflectionRay(intersectPoint + normal*1e-5, reflectDirection);

	vec3 reflectColor = RayTracing(node, reflectionRay, materials, lightpos, lightColor, depth - 1);
	color = (1.0f - materials[materialindex].reflect) * color + materials[materialindex].reflect * reflectColor;

    return color;
}

int main()
{
    // Initial
	const clock_t begin_time = clock();
	
    vec3 eye;
    vec3 viewdirection;
    vec3 upward(0, 1, 0);
    vec3 lightpos;
    vec3 lightColor(1, 1, 1);
    
    float angleOfView = 0.0;
    int width = 0, height = 0;
    
    vector<Sphere> spheres;
    vector<Triangle> triangles;
    vector<Material> materials;
    
    // Read from file
    ifstream file ("Input_Suzanne.txt");
//    ifstream file ("Input_Bunny.txt");
    if(file.is_open())
    {
        while(!file.eof()) {
            char type = ' ';
            file >> type;
            if(type == 'E') {
                float a, b ,c;
                file >> a >> b >> c;
                vec3 v(a, b, c);
                eye = v;
            }
            else if(type == 'V') {
                float a, b ,c;
                file >> a >> b >> c;
                vec3 v(a, b, c);
                viewdirection = v;
            }
            else if(type == 'F') {
                file >> angleOfView;
            }
            else if(type == 'R') {
                file >> width >> height;
            }
            else if(type == 'S') {
                float a, b, c, r;
                file >> a >> b >> c >> r;
                vec3 v(a, b, c);
                int mat = int(materials.size()-1);
                spheres.push_back(Sphere(v, r, mat));
            }
            else if(type == 'T') {
                vec3 points[3];
                vec3 nor, cen(0.0f, 0.0f, 0.0f);
                float a, b, c;
                for (int i=0; i<3; ++i) {
                    file >> a >> b >> c;
                    points[i] = vec3(a, b, c);
					cen += points[i];
                }
				cen /= 3;
                file >> a >> b >> c;
                nor = vec3(a, b, c);				
                int mat = int(materials.size()-1);
                triangles.push_back(Triangle(points, nor, cen, mat));
            }
            else if(type == 'M') {
                float a, b, c, d, e, f;
                float exp, r1, r2, nr;
                file >> a >> b >> c >> d >> e >> f >> exp >> r1 >> r2 >> nr;
                vec3 rgb(a, b, c), coef(d, e, f);
                materials.push_back(Material(rgb, coef, exp, r1, r2, nr));
            }
            else if(type == 'L') {
                float a, b ,c;
                file >> a >> b >> c;
                vec3 v(a, b, c);
                lightpos = v;
            }
        }
    }
    else {
        cout << "File Error!" << endl;
    }
    file.close();
    // center postiton
    vec3 screenCenter = eye + viewdirection.normalize();
    
    // X, Y axis
    vec3 xAxis = viewdirection ^ upward;
    vec3 yAxis = viewdirection ^ xAxis;
    float halfScreenX = tan(angleOfView/2);
    float halfScreenY = halfScreenX * height / width;
    
    vec3 upLeft(screenCenter - xAxis.normalize()*halfScreenX - yAxis.normalize()*halfScreenY);
    vec3 upRight(screenCenter + xAxis.normalize()*halfScreenX - yAxis.normalize()*halfScreenY);
    vec3 downLeft(screenCenter - xAxis.normalize()*halfScreenX + yAxis.normalize()*halfScreenY);
    vec3 downRight(screenCenter + xAxis.normalize()*halfScreenX + yAxis.normalize()*halfScreenY);
    
    OctNode * mainTriangles = new OctNode();
	cout << triangles.size() << endl;
    mainTriangles = mainTriangles->build(triangles, 0);
	cout << "Build finish time "<< float(clock() - begin_time) / CLOCKS_PER_SEC  << endl;
    // pixel color for height * width
    vector<vector<vec3>> color;

    for (int i=0; i<height; i+=1) {
        color.push_back(vector<vec3>());
        
        for(int j=0; j<width; j+=1) {
			if (i == height / 2 && j == width / 2)
				cout << "test" << endl;
            vec3 up((upLeft*(width - j) + upRight*(j)) / width);
            vec3 down((downLeft*(width - j) + downRight*(j)) / width);

            vec3 current((up * (height - i) + down * i) / height);

            Ray ray(eye, (current - eye).normalize());

            vec3 illumination(0, 0, 0);
            // raytracing with triangle
            illumination = RayTracing(mainTriangles, ray, materials, lightpos, lightColor, 20); // reflection for 20 times
            color.back().push_back(illumination*255);
            
        }
    }
    
    // draw image
    ColorImage image;
    
    image.init(width, height);
    for (int y=0; y<height; y++) {
        for (int x=0; x<width; x++) {
            Pixel rgb = {static_cast<unsigned char>(color[y][x][0]), static_cast<unsigned char>(color[y][x][1]), static_cast<unsigned char>(color[y][x][2])};
            image.writePixel(x, y, rgb);
        }
    }
    
//    image.outputPPM("octree bunny shadow 256.ppm");
    image.outputPPM("octree monkey shadow 128.ppm");
    std::cout << float(clock() - begin_time) / CLOCKS_PER_SEC << endl;
//    system("pause");
	return 0;
}
