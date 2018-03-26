#ifndef CAR_H_
#define CAR_H_

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <vector>
#include <string>
#include "rectangle.h"
#include "circle.h"
#include "cannonball.h"

using namespace std;

class Car {
	public: 
		Car();
		Point center;
		float radius;
		string id;
		float carspeed, shotspeed, shotsize;
		float carangle, wheelangle, cannonangle;

		Point getCenter();
		float getRadius();
		float getBodyHeight();
		float getBodyWidth();
		string getId();
		float getCarSpeed();
		float getShotSize();
		float getShotSpeed();
		float getCarAngle();
		float getWheelAngle();
		float getCannonAngle();

		void setCenter(Point center);
		void setRadius(float radius);
		void setId(string id);
		void setCarSpeed(float carspeed);
		void setShotSpeed(float shotspeed);
		void setShotSize(float shotsize);
		void setCarAngle(float angle);
		void setWheelAngle(float angle);
		void setCannonAngle(float angle);
		void moveWheel(float x);
		void forwardMove(float speed);
		void backwardMove(float speed);
		void moveCannon(float angle);
		void displayCircle(float radius, float r, float g, float b);
    	void displayRectangle(float height, float width, float r, float g, float b);
    	void displayWheelGrooveMotion();
    	void displayWheelGrooveStop();

		void displayCar();
};

#endif /* CAR_H_ */