#pragma once
#ifndef Light_h
#define Light_h
#include "vec3.h"
#include "shape.h"
#include "shapeList.h"
#include "ray.h"


class Light {//use as a parent class if I implement more than one light type
public:	
	vec3 d; //light direction
	float i; //light intensity
	float r;
	float g;
	float b;
	virtual bool shadowCheck(ray& r, vec3& normal, shapeList& shapeRecord);
};

bool Light::shadowCheck(ray& r, vec3& normal, shapeList& shapeRecord) { return true; };
#endif // !Light_h
