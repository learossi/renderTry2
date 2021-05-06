#pragma once
#ifndef shapeList_h
#define shapeList_h
//#include "shape.h"
#include "ray.h"

class shapeList : public shape {
public:
	shapeList(shape **list, int length) {
		//takes a pointer array of primitive objects (declared in main right now) and an integer which is the number of objects in that array

		l = length;
		allShapes = list; 
		minD = 0;
		shapeNumRec = -1;
		scale = 0;
	};

	virtual bool intersect(ray& r, float& scaleOut);
	int shapeNumRec;
	float scale=0;
	shape** allShapes;

private:
	int l;
	float minD;
};

bool shapeList::intersect(ray& r, float& scaleOut) {//need to figure out if I want a parameter for the shape list
	//takes in a ray and a scale parameter which is only another way to output: returns true if ray intersects with any object in shapeList
	shapeNumRec = -1;
	for (int i = 0; i < l; i++) {
		if (allShapes[i]->intersect(r, this->scale)) {
			if (shapeNumRec == -1) {//checks if this is first intersection
				shapeNumRec = i;
				minD = scale;
			}else if (scale < minD) {//checks if intersection is nearer than previous nearest intersection
				shapeNumRec = i;
				minD = scale;
			};
		};
	};
	if (shapeNumRec != -1) { //if there was an intersection
		r.s = minD; //find nearest intersected object
		scaleOut = minD;
		return true;
	}
	else {
		return false;
	}

};
#endif // !shapeList_h
