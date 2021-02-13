#include <iostream>
#include <fstream>
#include "vec3.h"
#include "sphere.h"
#include "ray.h"
#include "shape.h"
#include "shapeList.h"
#include "dLight.h"
#include "triangle.h"

using namespace std;

//variable decs
int sphere::sphereTotal = 0;
int shape::rayDepth = 4;
int shape::raysPerLevel=4;

//function decs
void sphereDec();

//todo: add shadow rays to color functions (custom function to return once one hit has happened?), add recursive cast rays to color,
//materials, multithreading???

int main() {
	int xDim=100;
	int yDim=100;
	float pixelDim = .015; 
	vec3 bgColor = vec3(.68627, .78431, .88235);
	int antiAliasingRay = 0;
	float antiAliasingRange = .7;
	float gamma = .4;

	shape* record[3];
	record[0] = new sphere(50, vec3(50, 50, 200), .7); //look at this and make sure I'm not fucking anything up
	record[1] = new sphere(70, vec3(-200, 0, 400), .6);
	record[2] = new sphere(1800, vec3(0, -1800, 900), .1);
	/*record[0] = new sphere(50, vec3(50, 50, 200)); //look at this and make sure I'm not fucking anything up
	record[1] = new sphere(70, vec3(-200, 0, 400));
	record[2] = new sphere(2000, vec3(1100, -200, 2500));*/
	shapeList list = shapeList(record, 3);
	dLight primaryLight = dLight(vec3(0, -2, 1), .3);


	ofstream cout("render.ppm");
	cout << "P3\n" << xDim << " " << yDim << "\n" << "255\n";

	for (int i = 0; i < yDim; i++) {
		for (int j = 0; j < xDim; j++) {
			vec3 colorRecord = vec3(0, 0, 0);
			bool hasHit = false;
			for (int k = 0; k <= antiAliasingRay; k++) {
				vec3 direction = vec3(j * pixelDim - (xDim * pixelDim / 2), 
					yDim*pixelDim-i * pixelDim - (yDim * pixelDim / 2) , 
					1); //generating small displacements on rays
				if (k > 0) {
					direction += vec3((((float)rand() * (antiAliasingRange * 2) / RAND_MAX) - antiAliasingRange) * pixelDim,
						(((float)rand()*(antiAliasingRange*2) / RAND_MAX) - antiAliasingRange) * pixelDim, 0);
				};
				vec3 origin = vec3();
				ray castRay(origin, direction);
				vec3 rayTest = castRay.d / castRay.d.z();
				if (i == 56 && j == 26) {
					int i = 50;
				};
				float scaleReturn = 0;
				bool hasIntersect = list.intersect(castRay, scaleReturn);
				if (hasIntersect == true) {
					vec3 color = record[list.shapeNumRec]->color(castRay, primaryLight, list.shapeNumRec, list);
					colorRecord += color;
					hasHit = true;
				}
				else {
					colorRecord += bgColor;
				};
			}
			if (hasHit == false) {
				cout << ceil(bgColor.x()*255) << " " << ceil(bgColor.y()*255) << " " << ceil(bgColor.z()*255) << "  ";
			}
			else {
				colorRecord = colorRecord / (antiAliasingRay + 1);
				if (colorRecord.x() >= 1) {//breakpoint catcher
					int test = 0;
				}
				
				cout << floor(pow(std::min(colorRecord.x(), float(1.0)),gamma)*255) << " " << 
					floor(pow(std::min(colorRecord.y(), float(1.0)),gamma) * 255) << " "
					<< floor(pow(std::min(colorRecord.z(), float(1.0)),gamma) * 255) << "  ";
				cerr << "(" << xDim * i + j << "/" << xDim * yDim << ")" << endl;
			}

		};
		cout << endl;

	};

	return 0;
};

void sphereDec() {

};