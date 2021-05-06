#pragma once
#ifndef ray_h
#define ray_h
#include "vec3.h"

class ray {
public:
	vec3 o; //origin, direction, and scale parameters of the ray
	vec3 d;
	float s;

	ray(vec3& orig, vec3& direc) {
		o = orig;
		d = direc/(direc.magn());
		s = 1;
	};
	ray() {o = vec3(); d = vec3();};

	vec3 origin() { return o; }
	vec3 direction() { return d; }

	vec3 scale(float c) { return o + d * c; };
	vec3 finalRay() { return o + d * s; }; //returns the final location of the ray
};


#endif // !ray_h
