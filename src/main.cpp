/*
 * main.cpp
 *
 *  Created on: 02.02.2012
 *      Author: yoshi252
 */

#include "BlobbyThingGame.h"

#include "common.h"

#include "MultiSpline.h"

int main(int argc, char* argv[]) {

//	srand(time(NULL));
//
//	const int length = 1234567;
//
//	Spline3d s;
//
//	std::vector<Vector4d> vec;
//	double pos = 0;
//	for (int i = 0; i < length; i++) {
//		double x1 = (rand()%1000) / 1000.0;
//		double x2 = pos;
//		double x3 = (i%2 == 0)?0:-1;
//		vec.push_back(Vector4d(pos,x1,x2,x3));
//		pos += (rand() % 10000 + 1) / 10000.0;
//	}
////	vec.push_back(Vector4d(1,1,1, 1));
////	vec.push_back(Vector4d(1.1,5,3, 2));
////	vec.push_back(Vector4d(2.1,5,3, 2));
////	vec.push_back(Vector4d(3,2,2, 66));
////	vec.push_back(Vector4d(4,0,1, 7));
////	vec.push_back(Vector4d(5,1,0, 19));
////	vec.push_back(Vector4d(6,1,-1, 0));
//
//	uint32_t startTime = SDL_GetTicks();
//	s.setPoints(vec);
//	uint32_t endTime = SDL_GetTicks();
//
//	LOG("Operation took ", (endTime - startTime) / 1000.0," seconds. That is ", int(length / ((endTime - startTime) / 1000.0)), " per second.");
//
////	for(double d = 0; d < 5; d+=0.01) {
////		Vector3d p = s.evaluate(d);
////		std::cout << d << ", " << p(0) << ", " << p(1) << ", " << p(2) << std::endl;
////	}
//
//	return 0;

	BlobbyThingGame game(1280, 720, 24);
	int ret = game.run();
	return ret;
}
