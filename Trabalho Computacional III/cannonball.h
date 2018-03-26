#ifndef CANNONBALL_H_
#define CANNONBALL_H_

#include "circle.h"

using namespace std;

class Cannonball {
	public: 
		float x, y, carRadius;
		float shotspeed, size; 
		float carangle, cannonangle;

		Cannonball(float x, float y, float carRadius, float shotspeed, float size, float carangle, float cannonangle);
		float getX();
		float getY();
		float getCarRadius();
		float getShotSpeed();
		float getShotSize();
		float getCarAngle();
		float getCannonAngle();

		void setX(float x);
		void setY(float y);
		void setCarRadius(float radius);
		void setShotSpeed(float shotspeed);
		void setSize(float size);
		void setCarAngle(float angle);
		void setCannonAngle(float angle);
		void displayCircle(float radius, float r, float g, float b);
		void displayCannonball();
		void updateCannonball(GLdouble timeDiff);
};

#endif /* CANNONBALL_H_ */