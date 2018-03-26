#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "cannonball.h"
#include <stdio.h>
#include <math.h>

using namespace std;

extern GLfloat window_size;
	
Cannonball::Cannonball(float x, float y, float carRadius, float shotspeed, float size, float carangle, float cannonangle) {
	this->x = x;
	this->y = y;
	this->carRadius = carRadius;
	this->shotspeed = shotspeed;
	this->size = size;
	this->carangle = carangle;
	this->cannonangle = cannonangle;
}
float Cannonball::getX() {
	return x;
}
float Cannonball::getY() {
	return y;
}
float Cannonball::getCarRadius() {
	return carRadius;
}
float Cannonball::getShotSpeed() {
	return shotspeed;
}
float Cannonball::getShotSize() {
	return size;
}
float Cannonball::getCarAngle() {
	return carangle;
}
float Cannonball::getCannonAngle() {
	return cannonangle;
}
void Cannonball::setX(float x) {
	this->x = x;
}
void Cannonball::setY(float y) {
	this->y = y;
}
void Cannonball::setCarRadius(float radius) {
	carRadius = radius;
}
void Cannonball::setShotSpeed(float shotspeed) {
	this->shotspeed = shotspeed;
}
void Cannonball::setSize(float size) {
	this->size = size;
}
void Cannonball::setCarAngle(float angle) {
	carangle = angle;
}
void Cannonball::setCannonAngle(float angle) {
	cannonangle = angle;
}
void Cannonball::displayCircle(float radius, float r, float g, float b) {
    
    int triangleAmount = 70;
    float twicePi = 2.0f * PI;

    glBegin(GL_TRIANGLE_FAN);
    	glColor3f(r, g, b);
        glVertex2f(this->x, window_size - this->y);
 
        for(int i = 0; i <= triangleAmount; i++)  
            glVertex2f(this->x + (radius * cos(i *  twicePi / triangleAmount)), 
                           window_size - this->y + (radius * sin(i * twicePi / triangleAmount)));
        
    glEnd();
}
void Cannonball::displayCannonball() {

	glPushMatrix();
			glTranslated(-2.0, 0.7*this->getCarRadius(), 0.0);
        	displayCircle(0.07*this->getCarRadius(), 1.0, 1.0, 0.0);
		glPopMatrix();
}
void Cannonball::updateCannonball(GLdouble timeDiff) {
	this->x += -timeDiff * this->getShotSpeed() * sin((carangle + cannonangle) * PI/180.0);
	this->y += -timeDiff * this->getShotSpeed() *  cos((carangle + cannonangle) * PI/180.0);
}