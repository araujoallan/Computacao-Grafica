#ifndef CAR_H_
#define CAR_H_

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <vector>
#include <iterator> 
#include <string>
#include "rectangle.h"
#include "circle.h"
#include "cannonball.h"

using namespace std;

class Car {
	public: 
		Car();
		Circle *circle_ref;
		Point center;
		float radius;
		string fill, id;
		float carspeed, enemyspeed, shotspeed, shotsize, shotfrequence;
		float carangle, wheelangle, cannonangle;
		bool moveflag;

		Circle* getCircleRef();
		friend class Point;
		Point getCenter();
		float getRadius();
		float getBodyHeight();
		float getBodyWidth();
		string getFill();
		string getId();
		float getCarSpeed();
		float getEnemySpeed();
		float getShotSize();
		float getShotSpeed();
		float getShotFrequence();
		float getCarAngle();
		float getWheelAngle();
		float getCannonAngle();
		bool getMoveFlag();

		void setCircleRef(Circle *circle);
		void setCenter(Point center);
		void setRadius(float radius);
		void setFill(string fill);
		void setId(string id);
		void setCarSpeed(float carspeed);
		void setEnemySpeed(float enemyspeed);

		void setShotSpeed(float shotspeed);
		void setShotFrequence(float shotfrequence);
		void setShotSize(float shotsize);
		void setCarAngle(float angle);
		void setWheelAngle(float angle);
		void setCannonAngle(float angle);
		void setMoveFlag(bool value);
		void moveWheel(float x);
		void forwardMove(float speed);
		void backwardMove(float speed);
		void colorCheck(string color);
		void moveCannon(float angle);
		void displayCircle(float radius, float r, float g, float b);
		void displayBody(float height, float width, string fill);
    	void displayRectangle(float height, float width, float r, float g, float b);
    	void displayWheelGrooveMotion();
    	void displayWheelGrooveStop();
    	void displayWheelGroove();
		void displayCar();

		int crashCheck(Circle *circle);
		int hitByShot(Cannonball cannonball);
		int crashCheckBoard(Circle *circle);
		int crashCheck(Car *enemy);
		void shoot(vector<Cannonball>& shotlist);
		//TODO
		int enemyHitByShot();
		
		~Car();

};

#endif /* CAR_H_ */