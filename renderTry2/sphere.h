#pragma once
#ifndef sphere_h
#define sphere_h
#include "vec3.h"
#include "Light.h"
#include "ray.h"

class sphere: public shape {
public:
	static int sphereTotal;

	sphere(const float& radius, const vec3& center, const int mat = 1, const float albedo = 1, const float red = 1, const float green = 1, const float blue = 1) {
		rad = radius; c = center; sphereTotal = sphereTotal++; sphereNum = sphereTotal; r = red; g = green; b = blue; a = albedo; m = mat;
	};

	vec3 center() { return c; }
	float radius() { return rad; }
	virtual vec3 normal(ray& r);
	virtual bool intersect(ray& r, float& scaleOut);
	virtual vec3 color(ray& r, Light& l, const int& listNum, shapeList& shapeRecord, bool scatter=false);
	virtual vec3 globalIllum(ray& r, Light& l, const int& listNum, shapeList& list, int& depth);

private:
	float rad;
	vec3 c;
	int sphereNum=0;
	float r;
	float g;
	float b;
	float a;
	int m;
};

bool sphere::intersect(ray& r, float& scaleOut) {
	bool intersected = false;
	float rayProjMag = (c - r.o).dot(r.d);
	vec3 offset = r.d * rayProjMag - (c - r.o);
	if (rayProjMag > 0) {
		if (rad > offset.magn()) {
			intersected = true;
			scaleOut = rayProjMag - (sqrt(rad * rad - offset.magn() * offset.magn()));
			r.s = scaleOut;
		}
		else {
			scaleOut = 0;
			r.s = 0;
		};
	}
	return intersected;
};

vec3 sphere::color(ray& r, Light& l, const int& listNum, shapeList& shapesRecord, bool scatter) {
	//takes in the ray, the singular light, a reference to the int which represents the member of shapeList to color, and shapeList; spits out the color in rgb
	vec3 sNormal = (r.d * r.s-c).norm();//returns the normal to the sphere
	vec3 colorRec = vec3();
	int depth = 1;
	if (m == 1) {
		if (scatter == true) {
			vec3 globalColor = globalIllum(r, l, listNum, shapesRecord, depth);
			colorRec += globalColor;
		}
		if (l.shadowCheck(r, sNormal, shapesRecord)) {//this shadow check is redundant if I'm calling it from global illum
			vec3 lDirec = l.lightDirectionReturn(r.finalRay());
			colorRec += vec3((sNormal.x() * lDirec.x() * -1.0 + sNormal.y() * lDirec.y() * -1.0 +
				sNormal.z() * lDirec.z() * -1.0) * a / 3.14 * l.i * l.r * this->r,
				(sNormal.x() * lDirec.x() * -1.0 + sNormal.y() * lDirec.y() * -1.0 +
					sNormal.z() * lDirec.z() * -1.0) * a / 3.14 * l.i * l.g * this->g,
				(sNormal.x() * lDirec.x() * -1.0 + sNormal.y() * lDirec.y() * -1.0 +
					sNormal.z() * lDirec.z() * -1.0) * a / 3.14 * l.i * l.b * this->b);
		};
	}
	else if (m == 2) { //reflective material
		vec3 RtoNdist = r.direction() - sNormal*sNormal.dot(r.direction()); //will need to rewrite if I want changeable camera
		vec3 complementRayD = sNormal + RtoNdist ;
		int intersectTest = 0;
		vec3 intersectPoint = r.finalRay();
		float scalePlaceholder = 0;
		ray reflectionRay = ray(intersectPoint, complementRayD);
			while (intersectTest == 0) {//iterate until I hit an object that isn't reflective or until no hit, at which point, globalIllum
				bool intersectCheck = shapesRecord.intersect(reflectionRay, scalePlaceholder); //running this intersect might screw things up for globalIllum
				if (intersectCheck == false) {
					colorRec += bgC*(vec3(this->r, this->g, this-> b)*a); //color the bckgrnd based on the sphere tint
					intersectTest = 1;
				}else {
					colorRec += shapesRecord.allShapes[shapesRecord.shapeNumRec]->color(reflectionRay,l,listNum,shapesRecord) 
						* (vec3(this->r, this->g, this->b) * a); //gets color of intersected object from reflectin ray (recursive if objects hit are reflective
					intersectTest = 1;
				}
			}
	}

	return vec3(std::min(float(1.0), std::max(float(0.0), colorRec.x())),
		std::min(float(1.0), std::max(float(0.0), colorRec.y())),
		std::min(float(1.0), std::max(float(0.0), colorRec.z())));
};

vec3 sphere::globalIllum(ray& r, Light& l, const int& listNum, shapeList& list, int& depth) {
	//takes in the same as color plus a reference to an int depth which determines when it terminates; spits out the color at the end
	vec3 colorAverage = vec3();
	int intersectedObj = list.shapeNumRec; //from previous time interesected was run - this implementation is so hacky, I hate it
	for (int i = 0; i < raysPerLevel; i++) {
		
		if (depth == rayDepth) {//add the shadow ray value to colorAverage - only runs if we're at the final ray depth
			float intersectArgPlaceholder = 0;
			bool scatterIntersectTest = list.intersect(r, intersectArgPlaceholder);
			if (scatterIntersectTest == true) {
				int scatterRayObj = list.shapeNumRec;
				vec3 hitObjectNormal = list.allShapes[scatterRayObj]->normal(r);
				colorAverage += color(r, l, scatterRayObj, list, false);
				return colorAverage;
			}
			else {
				return colorAverage;
			}
		}
		else {//add shadow ray value averaged with other global illum call with incremented depth to colorAverage
			colorAverage += color(r, l, listNum, list, false); //here's my problem here (update: this problem is still not fixed)
			for (int j = 0; j < raysPerLevel; j++) {
				
				vec3 intersectedObjNormal = list.allShapes[intersectedObj]->normal(r);
				vec3 scatterOrigin = r.finalRay(); //place where the ray passed into globalIllum intersected
				vec3 scatterDirection = vec3(); //generate random direction
				do
				{
					scatterDirection = vec3((float)rand()/RAND_MAX, (float)rand()/RAND_MAX, (float)rand()/RAND_MAX); //scatter in a random direction
				} while (sqrt(scatterDirection.x()*scatterDirection.x()+ scatterDirection.y()* scatterDirection.y()+
					scatterDirection.z()* scatterDirection.z())>1); //make sure the scatter direction is reasonable
				vec3 scatterRayFinalDirec = intersectedObjNormal + scatterDirection; //add Obj and scatter direction to simulate lambertian
				ray scatterRay = ray(scatterOrigin, scatterRayFinalDirec);
				float intersectArgPlaceholder = 0;
				bool scatterIntersectTest = list.intersect(scatterRay, intersectArgPlaceholder);
				if (scatterIntersectTest == true) {
					int scatterRayObj = list.shapeNumRec; //updated for new intersect
					int depthCounter = depth + 1;
					colorAverage += globalIllum(scatterRay, l, scatterRayObj, list, depthCounter)*this->a; //recur
				}
			}
		}
	};
	return colorAverage/raysPerLevel;
};

vec3 sphere::normal(ray& r) {
	return (r.finalRay() - c).norm();
};

#endif // !sphere_h
