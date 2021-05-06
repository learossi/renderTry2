#pragma once
#ifndef dLight_h
#define dLight_h
#include "vec3.h"
#include "Light.h"
#include "ray.h"
#include "shape.h"
//#include "shapeList.h"

class dLight:public Light {
public:
	dLight(vec3 direction, float intensity, const float red = 1, const float green = 1, const float blue = 1) {
		d = direction;
		i = intensity;
		r = red;
		g = green;
		b = blue; 
	};
	virtual bool shadowCheck(ray& r, vec3& normal, shapeList& shapeRecord);
	virtual vec3 lightDirectionReturn(vec3 intersectionPoint);
	vec3 LColor();

};

vec3 dLight::LColor() { return vec3(r,g,b); };
bool dLight::shadowCheck(ray& r, vec3& normal, shapeList& shapeRecord) {//maybe implement something like shapeList to loop over all lights if I get to adding that
	//need to check that intersections with rays of nonzero origin are well behaved
	vec3 shadowRayO = r.finalRay() + (normal * .1); //final term is the shadow bias
	vec3 shadowRayD = d * -1;
	ray shadowRay = ray(shadowRayO, shadowRayD);
	float scaleInput = 0;
	if (shapeRecord.intersect(shadowRay,scaleInput)==true) {
		//also might consider writing separate method in shapelist that returns true as soon as it intersects, much more efficient
		return false;
	}
	else {
		return true;
	}
};
vec3 dLight::lightDirectionReturn(vec3 intersectionPoint) { return d; }

#endif // !dLight_h
