#pragma once
#ifndef vec3_h
#define vec3_h

#include <cmath>
#include <iostream>

class vec3 {
public:
	vec3() {c[0] = 0; c[1] = 0; c[2] = 0;};
	vec3(float x, float y, float z) {c[0] = { x }; c[1] = { y }; c[2] = { z };};

	float magn() {return sqrt(c[0] * c[0] + c[1] * c[1] + c[2] * c[2]);}; //returns the magnitude of the vector
	vec3 norm() {float magnitude = magn(); return vec3(c[0] / magnitude, c[1] / magnitude, c[2] / magnitude);}; //return the normal vector
	vec3 cross(vec3& v2) {//takes in one other vector, returns cross product
		return vec3(v2.c[2] * c[1] - c[2] * v2.c[1], c[2] * v2.c[0] - c[0] * v2.c[2], c[0] * v2.c[1] - c[1] * v2.c[0]);};
	float dot(vec3 v2) {//takes in one other vector, returns dot product
		return c[0] * v2.c[0] + c[1] * v2.c[1] + c[2] * v2.c[2];};
	float x() { return c[0]; };
	float y() { return c[1]; };
	float z() { return c[2]; };

	vec3 operator*(const vec3& v) {//makes multiplication multiply elements
		return vec3(c[0] * v.c[0], c[1] * v.c[1], c[2] * v.c[2]);};
	vec3 operator*(const float s) {//makes multiplication scale the vector
		return vec3(c[0] * s , c[1] * s , c[2] * s);};
	vec3 operator+(const vec3& v) {//addition over vectors
		return vec3(c[0] + v.c[0], c[1] + v.c[1], c[2] + v.c[2]);};
	vec3 operator-(const vec3& v) {//subtraction over vectors
		return vec3(c[0] - v.c[0], c[1] - v.c[1], c[2] - v.c[2]);};
	vec3 operator/(const vec3& v) {//division over vectors
		return vec3(c[0] / v.c[0], c[1] / v.c[1], c[2] / v.c[2]);};
	vec3 operator/(const float& v) {//division over vectors by scalars
		return vec3(c[0] / v, c[1] / v, c[2] / v);};
	void operator=(const vec3 v) {//equality
		c[0] = v.c[0]; c[1] = v.c[1]; c[2] = v.c[2];};
	void operator+=(const vec3 v) {//the addition reassignment operator over vectors
		c[0] = c[0] + v.c[0]; c[1] = c[1] + v.c[1]; c[2] = c[2] + v.c[2];}
	bool operator==(vec3 v) {//boolean equality test over vectors
		if (c[0] == v.x() && c[1] == v.y() && c[2] == v.z()) {return true;}
		else {return false;}}

private:
	float c[3] = {};
};





#endif // !vec3_h
