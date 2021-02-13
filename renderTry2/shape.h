#pragma once
#ifndef shape_h
#define shape_h
#include "ray.h"
#include "vec3.h"
//#include "Light.h"
//forward guidance on header files maybe

class Light;
class shapeList;

class shape {
public:
	static int rayDepth;
	static int raysPerLevel;
	virtual vec3 normal(ray& r);
	virtual bool intersect(ray& r, float& scaleOut);
	virtual vec3 color(ray& r, Light& l, const int& listNum, shapeList& shapesRecord, bool scatter=true);
	virtual vec3 globalIllum(ray& r, Light& l, const int& listNum, shapeList& list, int& depth);
};

vec3 shape::color(ray& r, Light& l, const int& listNum, shapeList& shapesRecord, bool scatter) {
	return vec3();
};
bool shape::intersect(ray& r, float& scaleOut) {
	return false;
};

vec3 shape::globalIllum(ray& r, Light& l, const int& listNum, shapeList& list, int& depth) {
	return vec3();
};

vec3 shape::normal(ray& r) {
	return vec3();
};

#endif // !shape_h
