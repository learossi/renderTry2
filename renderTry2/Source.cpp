#include <iostream>
#include <fstream>
#include <numbers>
#include "vec3.h"
#include "ray.h"

#include "shape.h"
#include "shapeList.h"
#include "triangle.h"
#include "sphere.h"

#include "dLight.h"
#include "pLight.h"

using namespace std;

//variable decs
vec3 shape::bgC = vec3(.68627, .78431, .88235); //background color used for reflections
int shape::rayDepth = 4; //number of times rays will bounce during global illumination
int shape::raysPerLevel=4; //number of rays that will scatter from each bounce


int main() {
	const float PI = 3.14159;
	int xDim=100; //# of pixels in x, used to set resolution
	int yDim=100; //# of pixels in y
	float fov =130; //field of view, in degrees
	vec3 bgColor = vec3(.68627, .78431, .88235); //can set background color here for when no intersectino happens
	int antiAliasingRay = 0; //# of antialiasing rays
	float antiAliasingRange = .7; //how wide the antialiasing rays spread
	float gamma = .6; //traditional image gamma, applied during antialiasing

	float pixelDim = 2 * tan((2 * PI * fov) / (2 * 360)) / xDim; //calculates pixel dimensions
	const int shapeAmount = 1; //number of shapes in the scene
	shape* record[shapeAmount];
	/*record[0] = new triangle(vec3(0,0,5), vec3(2,-5,6), vec3(3,2,9), vec3(0,0,0)); 
	record[1] = new triangle(vec3(2, -5, 6), vec3(5,-3,10), vec3(3, 2, 9),vec3(0, 0, 0));
	record[2] = new triangle(vec3(2, -5, 6), vec3(0, 0, 5), vec3(-4.4,-1,8.8), vec3(0, 0, 0));
	record[3] = new triangle(vec3(2, -5, 6), vec3(-4.4, -1, 8.8), vec3(-2.4, -6, 9.8), vec3(0, 0, 0));
	record[4] = new triangle(vec3(0, 0, 5), vec3(3,2,9), vec3(-1.4, 1, 12.4), vec3(0, 0, 0));
	record[5] = new triangle(vec3(-4.4, -1, 8.8), vec3(0, 0, 5), vec3(-1.4, 1, 12.4), vec3(0, 0, 0));*/
	/*record[0] = new sphere(20, vec3(5, 10, 30), 2);
	record[1] = new sphere(5, vec3(25, 0, 10));
	record[2] = new triangle(vec3(-30, -10, 0), vec3(30, -10, 0), vec3(-30, -10, 30), vec3(), 1, 1, .8,.8);
	record[3] = new triangle(vec3(30, -10, 0), vec3(30, -10, 30), vec3(-30, -10, 30), vec3(), 1, 1,.8,.8);
	record[4] = new sphere(6, vec3(-12, 5, 12), 1);*/
	record[0] = new triangle(vec3(0, 3, 5), vec3(0, -3, 5), vec3(4, 0, 5), vec3(-3,0,0));

	shapeList list = shapeList(record, shapeAmount); //initialize shape list

	dLight primaryLight = dLight(vec3(1, 0, 3), .3); //declare light


	ofstream cout("render.ppm"); //move cout to the ppm file, can change filename here
	cout << "P3\n" << xDim << " " << yDim << "\n" << "255\n"; //set header of the ppm here

	for (int i = 0; i < yDim; i++) {
		for (int j = 0; j < xDim; j++) {
			vec3 colorRecord = vec3(0, 0, 0); //record of the colors of each ray returned
			bool hasHit = false;
			for (int k = 0; k <= antiAliasingRay; k++) {
				vec3 direction = vec3(j * pixelDim - (xDim * pixelDim / 2), 
					yDim*pixelDim-i * pixelDim - (yDim * pixelDim / 2) , 
					1); //generating a ray that goes through the center of each pixel
				if (k > 0) {
					direction += vec3((((float)rand() * (antiAliasingRange * 2) / RAND_MAX) - antiAliasingRange) * pixelDim,
						(((float)rand()*(antiAliasingRange*2) / RAND_MAX) - antiAliasingRange) * pixelDim, 0); //generating small displacements on each ray
				};
				vec3 origin = vec3();
				ray castRay(origin, direction); //the ray being cast from the camera
				float scaleReturn = 0; //a parameter that records how long the ray is at intersection
				bool hasIntersect = list.intersect(castRay, scaleReturn); //checks the closest intersection of the ray
				if (hasIntersect == true) {
					vec3 color = record[list.shapeNumRec]->color(castRay, primaryLight, list.shapeNumRec, list); //colors based on hit shape and light and applies global illumination
					colorRecord += color;
					hasHit = true;
				}
				else {
					colorRecord += bgColor;
				};
			}
			if (hasHit == false) {
				cout << ceil(bgColor.x()*255) << " " << ceil(bgColor.y()*255) << " " << ceil(bgColor.z()*255) << "  "; //output background color
				cerr << "(" << xDim * i + j << "/" << xDim * yDim << ")" << endl; //output render proress
			}
			else {
				colorRecord = colorRecord / (antiAliasingRay + 1); //average the antialiansing rays
				
				cout << floor(pow(std::min(colorRecord.x(), float(1.0)),gamma)*255) << " " << 
					floor(pow(std::min(colorRecord.y(), float(1.0)),gamma) * 255) << " "
					<< floor(pow(std::min(colorRecord.z(), float(1.0)),gamma) * 255) << "  "; //clamp values and apply the gamma
				cerr << "(" << xDim * i + j << "/" << xDim * yDim << ")" << endl; //output render progress
			}

		};
		cout << endl;

	};

	return 0;
};
