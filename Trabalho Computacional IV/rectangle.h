#ifndef RECTANGLE_H_
#define RECTANGLE_H_

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <string>
#include "point.h"

using namespace std;

class Rectangle {
    public: Point center;
    public: float width, height, r, g, b;
    public: string fill, id;

	public:
		friend class Point;
		Point getCenter();
		float getWidth();
		float getHeight();
		string getFill();
		string getId();
		void setCenter(Point center);
		void setWidth(float width);
		void setHeight(float hight);
		void setFill(string fill);
		void setId(string id);	
		void colorCheck(string color);
		void displayRectangle(Rectangle *rect);
};

#endif /* RECTANGLE_H_ */