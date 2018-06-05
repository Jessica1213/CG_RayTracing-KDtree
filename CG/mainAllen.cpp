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

using namespace std;


float determinant(mat3 m)
{
    return m[0][0] * m[1][1] * m[2][2] +
           m[0][1] * m[1][2] * m[2][0] +
           m[0][2] * m[1][0] * m[2][1] -
           m[0][2] * m[1][1] * m[2][0] -
           m[0][1] * m[1][0] * m[2][2] -
           m[0][0] * m[1][2] * m[2][1];
}

class Ray
{
public:
    vec3 o,d;
    Ray(vec3 o, vec3 d) : o(o), d(d) {}
};

class Material
{
public:
    vec3 rgb;
    vec3 coef; //Ka, Kd, Ks are the coefficients of the ambient, diffuse, and specular
    double exp, reflect, refract, nr;

	Material()
	{}
    Material(vec3 color, vec3 cof, double e, double r1, double r2, double n) : coef(cof), exp(e), reflect(r1), refract(r2), nr(n)
    {
		rgb = color;
    }
};

class Sphere
{
private:
    vec3 center;
    double radius;

public:
    Material material;
    Sphere(vec3 v, double r, Material m) : center(v), radius(r), material(m) {}
    
    vec3 getNormal(vec3 pi) { return (pi - center).normalize(); }
    
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
		if (t1 < 1e-4 && t2 < 1e-4)
			return -1;
		else if (t1 > 0 && t2 < 0)
			return t1 * d.length();
		else if (t1 < 0 && t2 > 0)
			return t2 * d.length();
		else {
			double t = min(t1, t2);
			return t * d.length();
		}
    }
    
    void print()
    {
        cout << "Sphere Ox: " << center[0] << " Oy: " << center[1] << " Oz: " << center[2] << " radius: " << radius << endl;
    }
};

class Triangle {
private:
    vec3 points[3];

public:
    Material material;
    Triangle(vec3 points[], Material m) : material(m)
    {
        for (int i=0; i<3; ++i)
            this->points[i] = points[i];
    }
    
    vec3 getNormal()
    {
        vec3 v1 = points[1]-points[0];
        vec3 v2 = points[2]-points[0];
        return (v1^v2).normalize();
    }
    
    float isIntersect(Ray ray)
    {
        vec3 v0 = points[0];
        vec3 v1 = points[1]-points[0];
        vec3 v2 = points[2]-points[0];
        
        vec3 o = ray.o;
        vec3 d = ray.d;
        mat3 matrix(-d, v1, v2);
        matrix = matrix.transpose();
        if(determinant(matrix) == 0) return false;
        vec3 sol = matrix.inverse() * (o-v0);
		if (sol[0] > 0 && sol[1] + sol[2] <= 1 && sol[1] >= 0 && sol[2] >= 0 && sol[1] <= 1 && sol[2] <= 1) {
			return sol[0] * d.length();
		}
		else 
			return -1;
    }
};


vec3 phongColor(vec3 normal, vec3 lightdirection, vec3 viewdirection, Material material, vec3 lightColor)
{
    vec3 color;
    double Ia=0.0, Id=0.0, Is=0.0, illumination=0.0;
    for (int c=0; c<3; c++)
    {
        // ambient  ka*Ia
        Ia = material.coef[0] * lightColor[c];
        
        // diffuse component  => kd*(li* (N dot L))
        Id = MAX(material.coef[1] * lightColor[c] * (normal * lightdirection), 0);
        
        // specular component  => ks*(li*((N dot H) ^ exp), H=L+V
        vec3 h = (lightdirection+viewdirection).normalize();
        double exp = material.exp;
        Is = MAX(material.coef[2] * lightColor[c] * pow((normal*h), exp), 0);
        illumination = (Ia+Id+Is)*255;
        double p = illumination*material.rgb[c];
        if (p > 255) p = 255;
        color[c] = p;
    }
    return color;
}

pair<int, float> findNearestSphere(Ray ray, vector<Sphere> &spheres)
{
	int index = -1;
	float distance = 1e9;
	for (int i = 0; i<spheres.size(); ++i) {
		float dist = spheres[i].isIntersect(ray);
		if (dist > 0 && dist < distance) {
			index = i;
			distance = dist;
		}
	}

	if (index == -1)
		distance = -1.0f;
	return make_pair(index, distance);
}

pair<int, float> findNearestTriangel(Ray ray, vector<Triangle> &triangles)
{
	int index = -1;
	float distance = 1e9;
	vec3 intersectPoint;
	for (int i = 0; i<triangles.size(); ++i) {
		float dist = triangles[i].isIntersect(ray);
		if (dist > 0 && dist < distance) {
			index = i;
			distance = dist;
		}
	}

	if (index == -1)
		distance = -1.0f;
	return make_pair(index, distance);
}

void ClampColor(vec3 &color)
{
	for (int i = 0; i < 3; ++i)
		color[i] = max(min(color[i], 1.0f), 0.0f);
}

vec3 RayTracing(Ray &ray, vector<Sphere> &spheres, vector<Triangle> &triangles, vec3 &light, vec3 &lightColor, int depth)
{
	vec3 backgroundColor(0, 0, 0);
	if (depth == 0)
		return backgroundColor;

	// check if it is intersection with Sphere and triangle
	pair<int, float> sphereIntersect = findNearestSphere(ray, spheres);
	pair<int, float> triangleIntersect = findNearestTriangel(ray, triangles);
	
	vec3 intersectPoint, normal;
	Material material;
	vec3 color(0, 0, 0);

	// No intersect
	if (sphereIntersect.second < 0 && triangleIntersect.second < 0) {
		return backgroundColor;
	}
	else if (sphereIntersect.second > 0 ||
		(sphereIntersect.second > 0 && triangleIntersect.second > 0 && sphereIntersect.second < triangleIntersect.second)) {
		intersectPoint = ray.o + ray.d * sphereIntersect.second;
		Sphere &sphere = spheres[sphereIntersect.first];
		normal = sphere.getNormal(intersectPoint);
		material = sphere.material;
	}
	else if (triangleIntersect.second > 0 ||
		(sphereIntersect.second > 0 && triangleIntersect.second > 0 && sphereIntersect.second > triangleIntersect.second)) {
		intersectPoint = ray.o + ray.d * triangleIntersect.second;
		Triangle &triangle = triangles[triangleIntersect.first];
		normal = triangle.getNormal();
		// check normal
		if (normal * ray.d > 0)
			normal *= -1;
		material = triangle.material;
	}

	//color += triangle.material.rgb;
	// Ka * Ia
	color += material.coef[0] * lightColor;
	Ray towardLight(intersectPoint + normal*1e-5, (light - intersectPoint).normalize());
	if (findNearestSphere(towardLight, spheres).first == -1 &&
		findNearestTriangel(towardLight, triangles).first == -1) {
		// Kd * (Id)
		float NdotL = normal * towardLight.d;
		color += material.coef[1] * (lightColor * NdotL);
		// Ks * (Is)
		float NdotH = (towardLight.d - ray.d).normalize() * normal;
		if (NdotH > 0)
			color += material.coef[2] * (lightColor * pow(NdotH, material.exp));
	}

	color[0] *= material.rgb[0];
	color[1] *= material.rgb[1];
	color[2] *= material.rgb[2];

	// reflect
	vec3 reflectDirection = ray.d - 2 * (ray.d* normal) * normal;
	Ray reflect(intersectPoint + normal * 1e-5, reflectDirection.normalize());
	vec3 reflectColor = RayTracing(reflect, spheres, triangles, light, lightColor, depth - 1);
	color = (1.0f - material.reflect) * color + material.reflect * reflectColor;

	// refract
//	vec3 refractNormal = normal;
//	float c1 = ray.d * normal, snell = material.nr;
//	if (c1 < 0) {
//		snell = 1.0f / snell;
//		refractNormal *= -1;
//	}
//	float c2 = sqrt(1 - snell*snell*(1 - c1*c1));
//	vec3 refractDirection = snell * ray.d + (snell * c1 - c2) * normal;
//	Ray refract(intersectPoint - normal * 1e-5, refractDirection.normalize());
//	vec3 refractColor = RayTracing(refract, spheres, triangles, light, lightColor, depth - 1);
//	color = (1.0f - material.refract) * color + material.refract * refractColor;

	ClampColor(color);
	return color;
}

int main()
{
    // Initial
    vec3 eye;
    vec3 viewdirection;
    vec3 upward(0, 1, 0);
    vec3 light;
    vec3 lightColor(1, 1, 1);
    
    float angleOfView = 0.0;
    int width = 0, height = 0;
    
    vector<Sphere> spheres;
    vector<Triangle> triangles;
    vector<Material> materials;
    
    // Read from file
    ifstream file ("hw2_input.txt");
    if(file.is_open())
    {
        while(!file.eof()) {
            char type = ' ';
            file >> type;
            if(type == 'E') {
                int a, b ,c;
                file >> a >> b >> c;
                vec3 v(a, b, c);
                eye = v;
            }
            else if(type == 'V') {
                int a, b ,c;
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
                spheres.push_back(Sphere(v, r, materials.back()));
            }
            else if(type == 'T') {
                vec3 points[3];
                float a, b, c;
                for (int i=0; i<3; ++i) {
                    file >> a >> b >> c;
                    points[i] = vec3(a, b, c);
                }
                triangles.push_back(Triangle(points, materials.back()));
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
                light = v;
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
    
    
    // check intersection
    vector<vector<bool>> hasIntersection;
    
    // pixel color for height * width * rgb
    vector<vector<vec3>> color;
    
    for (int i=0; i<height; i+=1) {
        hasIntersection.push_back(vector<bool>());
        color.push_back(vector<vec3>());
        
        for(int j=0; j<width; j+=1) {
                        
            vec3 up((upLeft*(width - j) + upRight*(j)) / width);
            vec3 down((downLeft*(width - j) + downRight*(j)) / width);
            
            vec3 current((up * (height - i) + down * i) / height);
            
            vec3 lightdirection = (light-current).normalize();
            vec3 viewdirection = (eye-current).normalize();
            Ray ray(eye, (current - eye).normalize());
            
			vec3 illumination = RayTracing(ray, spheres, triangles, light, lightColor, 20);
			color.back().push_back(illumination * 255.0);
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
    
    image.outputPPM("Allenout.ppm");
    
    return 0;
}
