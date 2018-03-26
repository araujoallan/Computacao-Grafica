#ifndef CIRCLE_H_
#define CIRCLE_H_

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <string>
#include <math.h>
#include "point.h"

#define PI 3.14159265359

using namespace std;

class Circle {
    public: Point center;
    public: float radius;
    public: string fill, id;

	public:
		Circle();
		friend class Point;
		Point getCenter();
		float getRadius();
		string getFill();
		string getId();
		void setCenter(Point center);
		void setRadius(float radius);
		void setFill(string fill);
		void setId(string id);
    	void colorCheck(string color);
		void displayCircle(Circle *circle);
 		~Circle();
};

#endif /* CIRCLE_H_ */
