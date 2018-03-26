#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "cannonball.h"
#include <stdio.h>
#include <math.h>

using namespace std;

extern GLfloat window_size;

Cannonball::Cannonball(float x, float y, float carRadius, float shotspeed, float size, float carangle, float cannonangle) {
	
	this->carRadius = carRadius;
	this->shotspeed = shotspeed;
	this->size = size;
	this->carangle = carangle;
	this->cannonangle = cannonangle;

	carangle = carangle*PI/180.0;
	cannonangle = cannonangle*PI/180.0;
	
	//Matriz de Transformação
	float matrix_trans1[3][3] = { {cos(carangle), -sin(carangle), x}, 
						{sin(carangle), cos(carangle), y},
						{0, 0, 1} };
	//Ponto para mudança de base
	float base_point1[3] = {0, -carRadius/1.7, 1.0};

	//1a transf para base do canhao
	float matrix_mult1[3] = {0,0,0};
	for(int i = 0; i < 3; i++) {
		for(int j = 0; j < 3; j++)
			matrix_mult1[i] += matrix_trans1[i][j] * base_point1[j]; 
	}

	//2a transf ponta do canhao
	float matrix_trans2[3][3] = { {cos((carangle + cannonangle)), -sin((carangle + cannonangle)), matrix_mult1[0]}, 
						{sin((carangle + cannonangle)), cos((carangle + cannonangle)), matrix_mult1[1] },
						{0, 0, 1} };

	float base_point2[3] = {0, -(2.0/3.0)*carRadius/1.7, 1};
	float matrix_mult2[3] = {0,0,0};

	for(int i = 0; i < 3; i++) {
		for(int j = 0; j < 3; j++)
			matrix_mult2[i] += matrix_trans2[i][j] * base_point2[j]; 
	}		

	this->x = matrix_mult2[0];
	this->y = matrix_mult2[1];
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
	return 0.07*carRadius;
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
        	displayCircle(0.07*this->getCarRadius(), 1.0, 1.0, 0.0);
	glPopMatrix();
}
void Cannonball::updateCannonball(GLdouble timeDiff) {
	this->x += -timeDiff * this->getShotSpeed() * sin((carangle + cannonangle) * PI/180.0);
	this->y += -timeDiff * this->getShotSpeed() *  cos((carangle + cannonangle) * PI/180.0);
}
Cannonball::~Cannonball(void) {} 
