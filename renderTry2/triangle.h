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
	triangle(vec3 point1, vec3 point2, vec3 point3, float red = 1, float green = 1, float blue=1, float albedo=1) {
		vertexArray[0] = point1;
		vertexArray[1] = point2;
		vertexArray[2] = point3;
		edge1 = vertexArray[0] - vertexArray[1];
		edge2 = vertexArray[1] - vertexArray[2];
		edge3 = vertexArray[2] - vertexArray[0];
		r = red;
		g = green;
		b = blue;
		a = albedo;
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

};

bool triangle::intersect(ray& r, float& scaleOut) {
	vec3 planeNormal = normal(); 

	if (planeNormal.dot(r.direction()) == 0) { return false; }//check if parrallel

	float OriginPlaneDistance = vertexArray[0].dot(planeNormal); //distance to plane along normal from origin
	float planeInterScale = (planeNormal.dot(r.origin()) + OriginPlaneDistance) / planeNormal.dot(r.direction());

	if (planeInterScale < 0) { return false; }; //makes sure we don't return triangles behind ray

	vec3 planePoint = r.finalRay();

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
	vec3 sNormal = this->normal();//returns the normal to the triangle
	vec3 colorRec = vec3();
	int depth = 1;
	if (scatter == true) {
		vec3 globalColor = globalIllum(r, l, listNum, shapesRecord, depth);
		colorRec += globalColor;
	}

	if (l.shadowCheck(r, sNormal, shapesRecord)) {//this shadow check is redundant if I'm calling it from global illum; also, verify this works
		colorRec += vec3((sNormal.x() * l.d.x() * -1.0 + sNormal.y() * l.d.y() * -1.0 +
			sNormal.z() * l.d.z() * -1.0) * a / 3.14 * l.i * l.r * this->r,
			(sNormal.x() * l.d.x() * -1.0 + sNormal.y() * l.d.y() * -1.0 +
				sNormal.z() * l.d.z() * -1.0) * a / 3.14 * l.i * l.g * this->g,
			(sNormal.x() * l.d.x() * -1.0 + sNormal.y() * l.d.y() * -1.0 +
				sNormal.z() * l.d.z() * -1.0) * a / 3.14 * l.i * l.b * this->b);
	};


	return vec3(std::min(float(1.0), std::max(float(0.0), colorRec.x())),
		std::min(float(1.0), std::max(float(0.0), colorRec.y())),
		std::min(float(1.0), std::max(float(0.0), colorRec.z())));
};

vec3 triangle::globalIllum(ray& r, Light& l, const int& listNum, shapeList& list, int& depth) {
	return vec3();
};

#endif // !triangle_h
