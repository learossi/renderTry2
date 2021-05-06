#pragma once
#ifndef triangle_h
#define triangle_h
#include "vec3.h"
#include "shape.h"
#include "Light.h"
#include "ray.h"

class triangle: public shape {
public:
	triangle() {};
	triangle(vec3 point1, vec3 point2, vec3 point3, vec3 translation = vec3(0,0,0), const int mat =1, float red = 1, float green = 1, float blue=1, float albedo=1) {
		vertexArray[0] = point1+translation;
		vertexArray[1] = point2+translation;
		vertexArray[2] = point3+translation;
		edge1 = vertexArray[0] - vertexArray[1];
		edge2 = vertexArray[1] - vertexArray[2];
		edge3 = vertexArray[2] - vertexArray[0];
		r = red;
		g = green;
		b = blue;
		a = albedo;
		m = mat;
	};

	vec3 pOne() { return vertexArray[0]; };
	vec3 pTwo() { return vertexArray[1]; };
	vec3 pThree() { return vertexArray[2]; };
	virtual vec3 normal() { vec3 crossProd = edge1.cross(edge3); return crossProd / crossProd.magn(); };
	//this means it's a right handed coordinate system
	virtual bool intersect(ray& r, float& scaleOut);
	virtual vec3 color(ray& r, Light& l, const int& listNum, shapeList& shapesRecord, bool scatter = true);
	virtual vec3 globalIllum(ray& r, Light& l, const int& listNum, shapeList& list, int& depth);

	
private:
	vec3 vertexArray[3] = {vec3(), vec3(), vec3()};
	vec3 edge1;
	vec3 edge2;
	vec3 edge3;
	float r;
	float g;
	float b;
	float a;
	int m;
};

bool triangle::intersect(ray& r, float& scaleOut) {
	//takes in a reference to a ray and a float, spits out a boolean corresponding to whether it hit and scaleOut becomes the scalar by which to multiply the ray to get the hit point
	vec3 planeNormal = normal(); 

	if (planeNormal.dot(r.direction()) == 0) { return false; }//check if parrallel

	float OriginPlaneDistance = (vertexArray[0]- r.origin()).dot(planeNormal); //distance to plane along normal from origin of ray
	float planeInterScale = (planeNormal.dot(r.origin()) + OriginPlaneDistance) / planeNormal.dot(r.direction());

	if (planeInterScale < 0) { return false; }; //makes sure we don't return triangles behind ray

	r.s = planeInterScale;
	vec3 planePoint = r.finalRay();
	//vertex declarations - points must go counterclockwise
	vec3 P1 = planePoint - vertexArray[1];
	vec3 P2 = planePoint - vertexArray[2];
	vec3 P3 = planePoint - vertexArray[0];

	if (planeNormal.dot(edge1.cross(P1)) > 0 && planeNormal.dot(edge2.cross(P2)) > 0 
		&& planeNormal.dot(edge3.cross(P3)) > 0) {//3 edge test for intersection
		scaleOut = planeInterScale;
		r.s = planeInterScale;
		return true;
	}
	else {
		scaleOut = 0;
		r.s = 0;
		return false;
	}
};

vec3 triangle::color(ray& r, Light& l, const int& listNum, shapeList& shapesRecord, bool scatter) {
//takes in a ray reference, any light, the list num from shapes record, shapesrecord, and a bool to say if we should do global illumination
//returns the color of the triangle
	vec3 sNormal = this->normal();//returns the normal to the triangle
	vec3 colorRec = vec3();
	int depth = 1;
	if (m == 1) {
		if (scatter == true) {
			//vec3 globalColor = globalIllum(r, l, listNum, shapesRecord, depth);
			//colorRec += globalColor;
		}

		if (l.shadowCheck(r, sNormal, shapesRecord)) {//this shadow check is redundant if I'm calling it from global illum; also, verify this works
			vec3 lDirec = l.lightDirectionReturn(r.finalRay());
			colorRec += vec3((sNormal.x() * lDirec.x() * -1.0 + sNormal.y() * lDirec.y() * -1.0 +
				sNormal.z() * lDirec.z() * -1.0) * a / 3.14 * l.i * l.r * this->r,
				(sNormal.x() * lDirec.x() * -1.0 + sNormal.y() * lDirec.y() * -1.0 +
					sNormal.z() * lDirec.z() * -1.0) * a / 3.14 * l.i * l.g * this->g,
				(sNormal.x() * lDirec.x() * -1.0 + sNormal.y() * lDirec.y() * -1.0 +
					sNormal.z() * lDirec.z() * -1.0) * a / 3.14 * l.i * l.b * this->b);
		}
	}
	else if (m == 2) {
		vec3 RtoNdist = r.direction() - sNormal * sNormal.dot(r.direction()); //will need to rewrite if I want changeable camera
		//this is the distance between the tips of the normal and the ray assuming said tips lay on the same plane 
		vec3 complementRayD = sNormal + RtoNdist; //flips the ray around the normal for reflection
		int intersectTest = 0;
		vec3 intersectPoint = r.finalRay();
		float scalePlaceholder = 0;
		ray reflectionRay = ray(intersectPoint, complementRayD);
		while (intersectTest == 0) {//iterate until I hit an object that isn't reflective or until no hit, at which point, globalIllum
			bool intersectCheck = shapesRecord.intersect(reflectionRay, scalePlaceholder); //running this intersect might screw things up for globalIllum
			if (intersectCheck == false) {
				colorRec += bgC * (vec3(this->r, this->g, this->b) * a); //color the bckgrnd based on the triangle tint
				intersectTest = 1;
			}
			else {
				colorRec += shapesRecord.allShapes[shapesRecord.shapeNumRec]->color(reflectionRay, l, listNum, shapesRecord)
					* (vec3(this->r, this->g, this->b) * a); //gets color of intersected object from reflection ray (recursive if objects hit are reflective
				intersectTest = 1;
			}
		}
	}

	return vec3(std::min(float(1.0), std::max(float(0.0), colorRec.x())),
		std::min(float(1.0), std::max(float(0.0), colorRec.y())),
		std::min(float(1.0), std::max(float(0.0), colorRec.z())));
};

vec3 triangle::globalIllum(ray& r, Light& l, const int& listNum, shapeList& list, int& depth) {
	return vec3(); //not implemented
};

#endif // !triangle_h
