#pragma once
#ifndef ngon_h
#define ngon_h
#include "vec3.h"
#include "shape.h"
#include "triangle.h"

class ngon :public shape { //still need to actual run ngon and test for the inevitable bugs !!!!!!!!!!
	ngon(vec3 vertices[], shape** recordInput) {
		int vertexNumber = sizeof(vertices) / sizeof(*vertices);
		int recordNumber = sizeof(*recordInput) / sizeof(**recordInput);
		for (int i = 1; i < vertexNumber - 1; i++) {
			*recordInput[recordNumber + i] = triangle(*(vertices + i - 1), *(vertices + i), *(vertices + i + 1));
		}
	}
};

#endif // !ngon_h
