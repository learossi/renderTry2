#pragma once
#ifndef pLight_h
#define pLight_h
#include "vec3.h"
#include "Light.h"
#include "ray.h"
#include "shape.h"

class pLight : public Light {
public:
	pLight(vec3 origin, float intensity, const float red = 1, const float green = 1, const float blue = 1) {
		origin = origin;
		i = intensity;
		r = red;
		b = blue;
		g = green;

	};
	virtual vec3 lightDirectionReturn(vec3 intersectionPoint);
private:
	vec3 origin;
	vec3 rgb;
	
};

vec3 pLight::lightDirectionReturn(vec3 intersectionPoint) {
	return (origin - intersectionPoint)*-1; //returns vector between light and the point on the object
}
#endif // !pLight_h

