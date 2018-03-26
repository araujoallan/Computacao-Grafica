#include "car.h"
#include <stdio.h>
#include <math.h>

using namespace std;

extern GLfloat window_size;
extern bool moveflag;
extern vector<Car*> enemies;
extern vector<Cannonball> shotList;
GLfloat bodyheight, bodywidth, axleheight, axlewidth, wheelheight, wheelwidth, cannonheight, cannonwidth;

Car::Car() : carangle(0.0)  {}
//GETTERS
Circle* Car::getCircleRef() {
	return circle_ref;
}
Point Car::getCenter() {
	return center;
}
float Car::getRadius() {
	return radius;
}
float Car::getBodyHeight() {
	return this->getRadius()/1.7;
}
float Car::getBodyWidth() {
	return this->getRadius()/3.8;
}
string Car::getFill() {
	return fill;
}
string Car::getId() {
	return id;
}
float Car::getCarSpeed() {
	return carspeed;
}
float Car::getEnemySpeed() {
	return enemyspeed;
}
float Car::getShotSpeed() {
	return shotspeed;
}
float Car::getShotSize() {
	return shotsize;
}
float Car::getShotFrequence() {
	return shotfrequence;
}
float Car::getCarAngle() {
	return carangle;
}
float Car::getWheelAngle() {
	return wheelangle;
}
float Car::getCannonAngle() {
	return cannonangle;
}
bool Car::getMoveFlag() {
	return moveflag;
}
//SETTERS
void Car::setCircleRef(Circle *circle) {
	circle_ref = circle;
}
void Car::setCenter(Point center) {
	this->center = center;
}
void Car::setRadius(float radius) {
	this->radius = radius;
}
void Car::setFill(string fill) {
	this->fill = fill;
}
void Car::setId(string id) {
	this->id = id;
}
void Car::setCarSpeed(float carspeed) {
	this->carspeed = carspeed;
}
void Car::setEnemySpeed(float enemyspeed) {
	this->enemyspeed = enemyspeed;
}
void Car::setShotSpeed(float shotspeed) {
	this->shotspeed = shotspeed;
}
void Car::setShotSize(float shotsize) {
	this->shotsize = shotsize;
}
void Car::setShotFrequence(float shotfrequence) {
	this->shotfrequence = shotfrequence;
}

void Car::setCarAngle(float angle) {
	carangle = angle;
}
void Car::setWheelAngle(float angle) {
	wheelangle = angle;
}
void Car::setCannonAngle(float angle) {
	cannonangle = angle;
}
void Car::setMoveFlag(bool value) {
	this->moveflag = moveflag;
}
//METODOS
void Car::moveWheel(float angle) {
    wheelangle = wheelangle + angle;
}
void Car::moveCannon(float angle) {
	cannonangle = cannonangle + angle;
}
void Car::forwardMove(float speed) {
    carangle += 0.03*this->getWheelAngle();
	float aux = PI/2 + carangle*PI/180;
    
    this->center.y += speed*sin(aux);
    this->center.x -= speed*cos(aux);
}
void Car::backwardMove(float speed) {
    carangle -= 0.03*this->getWheelAngle();
	float aux = PI/2 + carangle*PI/180;
    
    this->center.y += speed*sin(aux);
    this->center.x -= speed*cos(aux);
}
void Car::colorCheck(string color) {

    if (color == "blue") 
        glColor3f(0.0, 0.0, 1.0);
    
    else if (color == "red") 
        glColor3f(1.0, 0.0, 0.0);
    
    else if (color == "green") 
        glColor3f(0.0, 1.0, 0.0);
    
    else if (color == "black") 
        glColor3f(0.0, 0.0, 0.0);
    
    else if (color == "white") 
        glColor3f(1.0, 1.0, 1.0);
}
void Car::displayCircle(float radius, float r, float g, float b) {
    
    int triangleAmount = 70;
    float twicePi = 2.0f * PI;

    glBegin(GL_TRIANGLE_FAN);
    	glColor3f(r, g, b);
        glVertex2f(this->center.x, window_size - this->center.y);
 
        for(int i = 0; i <= triangleAmount; i++)  
            glVertex2f(this->center.x + (radius * cos(i *  twicePi / triangleAmount)), 
                           window_size - this->center.y + (radius * sin(i * twicePi / triangleAmount)));
        
    glEnd();
}
void Car::displayRectangle(float height, float width, float r, float g, float b) {
    glColor3f(r , g, b);
    
    glBegin(GL_QUADS);
        glVertex2f(-width/2.0, 0.0);
        glVertex2f(-width/2.0, height);
        glVertex2f(width/2.0, height);
        glVertex2f(width/2.0, 0.0);
    glEnd();
}
void Car::displayBody(float height, float width, string fill) {
	this->colorCheck(fill);

    glBegin(GL_QUADS);
        glVertex2f(-width/2.0, 0.0);
        glVertex2f(-width/2.0, height);
        glVertex2f(width/2.0, height);
        glVertex2f(width/2.0, 0.0);
    glEnd();
}

void Car::displayWheelGrooveMotion() {
	this->displayRectangle(wheelheight, wheelwidth/5, 1.0, 1.0, 1.0);
}
void Car::displayWheelGrooveStop() {
	this->displayRectangle(wheelheight/4.5, wheelwidth/5, 1.0, 1.0, 1.0);
}
void Car::displayWheelGroove() {
	if(moveflag == true) 
				this->displayWheelGrooveMotion();
			else if(moveflag == false){
					glPushMatrix();
						glTranslatef(0.0, 0.8*wheelheight, 0.0);
						this->displayWheelGrooveStop();
						glTranslatef(0.0, -0.4*wheelheight, 0.0);
						this->displayWheelGrooveStop();
						glTranslatef(0.0, -0.4*wheelheight, 0.0);
						this->displayWheelGrooveStop();

					glPopMatrix();
			}
}
void Car::displayCar() {
	
	bodyheight = 2*this->getRadius()/1.7;
	bodywidth = 2*this->getRadius()/3.8;
	axleheight = bodyheight/10;
	axlewidth = bodywidth/2;
	wheelheight = bodyheight/2.5;
	wheelwidth = bodywidth/2;
	cannonheight = bodyheight/3;
	cannonwidth = bodywidth/5;

	glPushMatrix();
		//chassi do carro
		glTranslatef(this->getCenter().x, window_size - this->getCenter().y, 0.0);
		glRotatef(this->getCarAngle(), 0.0, 0.0, 1);
		glTranslatef(0.0, -0.5*bodyheight, 0.0); 
		this->displayBody(bodyheight, bodywidth, this->getFill());
		
		//piloto 
		glTranslatef(-this->getCenter().x, -(window_size - this->getCenter().y - 3*axleheight), 0.0);
		this->displayCircle(this->getRadius()/5, 0.0, 0.2, 0.4);
		glTranslatef(this->getCenter().x, (window_size - this->getCenter().y -3*axleheight), 0.0);

		//eixo dianeiro lado esquerdo
		glPushMatrix();
    		glTranslatef(-1.5*axlewidth, 8*axleheight, 0.0);
			this->displayRectangle(axleheight, axlewidth, 0.0, 0.5, 0.0);
    	//roda dianteira esquerda
			glTranslatef(-(axlewidth/2 + wheelwidth/2), axleheight/2, 0.0);
			glRotatef(this->getWheelAngle(), 0.0, 0.0, 1);
			glTranslatef(0.0, -0.5*wheelheight, 0.0);
			this->displayRectangle(wheelheight, wheelwidth, 0.0, 0.5, 0.0);
			
			this->displayWheelGroove();
			
		glPopMatrix();

		//eixo dianeiro lado direito
		glPushMatrix();
    		glTranslatef(1.5*axlewidth, 8*axleheight, 0.0);
			this->displayRectangle(axleheight, axlewidth, 0.0, 0.5, 0.0);
    	//roda dianteira direita
			glTranslatef(axlewidth/2 + wheelwidth/2, axleheight/2, 0.0);
			glRotatef(this->getWheelAngle(), 0.0, 0.0, 1);
			glTranslatef(0.0, -0.5*wheelheight, 0.0);
			this->displayRectangle(wheelheight, wheelwidth, 0.0, 0.5, 0.0);

			this->displayWheelGroove();
		
		glPopMatrix();

		//eixo traseiro lado esquerdo
		glPushMatrix();
    		glTranslatef(-1.5*axlewidth, axleheight, 0.0);
			this->displayRectangle(axleheight, axlewidth, 0.0, 0.5, 0.0);
    	//roda traseira esquerda
			glTranslatef(-wheelwidth, -1.5*axleheight, 0.0);
			this->displayRectangle(wheelheight, wheelwidth, 0.0, 0.5, 0.0);
			
			this->displayWheelGroove();

		glPopMatrix();

    	//eixo traseiro lado direito
    	glPushMatrix();
			glTranslatef(1.5*axlewidth, axleheight, 0.0);
			this->displayRectangle(axleheight, axlewidth, 0.0, 0.5, 0.0);
		//roda traseira direita
			glTranslatef(wheelwidth, -1.5*axleheight, 0.0);
			this->displayRectangle(wheelheight, wheelwidth, 0.0, 0.5, 0.0);
			
			this->displayWheelGroove();

		glPopMatrix();

		//canhao
		glPushMatrix(); 
			glTranslatef(0.0, bodyheight, 0.0);
			glRotatef(this->getCannonAngle(), 0.0, 0.0, 1);
			this->displayRectangle(cannonheight, cannonwidth, 0.0, 0.5, 0.0);
        glPopMatrix();

    glPopMatrix();
}
int Car::crashCheck(Circle *circle) {
    
    float cathetus_b = this->getCenter().getX() - circle->getCenter().getX();
    float cathetus_c = this->getCenter().getY() - circle->getCenter().getY();
    float distance = sqrt(cathetus_b*cathetus_b + cathetus_c*cathetus_c);

    if(distance <= (circle->getRadius() + this->getRadius()) ) {
        //Colision
        return 1;
    }
    return 0;
}
int Car::hitByShot(Cannonball cannonball) {
    
    float cathetus_b = this->getCenter().getX() - cannonball.x;
    float cathetus_c = this->getCenter().getY() - cannonball.y;
    float distance = sqrt(cathetus_b*cathetus_b + cathetus_c*cathetus_c);

    if(distance <= (cannonball.size + this->getRadius()) ) {
        //Colision
        return 1;
    }
    return 0;
}
int Car::crashCheckBoard(Circle *circle) {
    
    float cathetus_b = this->getCenter().getX() - circle->getCenter().getX();
    float cathetus_c = this->getCenter().getY() - circle->getCenter().getY();
    float distance = sqrt(cathetus_b*cathetus_b + cathetus_c*cathetus_c);

    if(distance > (circle->getRadius() - this->getRadius()) ) {
        //Colision
        return 1;
    }
    return 0;
}
int Car::crashCheck(Car *enemy) {
    
    float cathetus_b = this->getCenter().getX() - enemy->getCenter().getX();
    float cathetus_c = this->getCenter().getY() - enemy->getCenter().getY();
    float distance = sqrt(cathetus_b*cathetus_b + cathetus_c*cathetus_c);

    if(distance <= (enemy->getRadius() + this->getRadius()) ) {
        //Colision
        return 1;
    }
    return 0;
}
void Car::shoot(vector<Cannonball>& shotlist) {
	Cannonball *newshot = new Cannonball(this->getCenter().x, this->getCenter().y,  this->getRadius(), this->getShotSpeed(), this->getShotSize(), 
                                                 this->getCarAngle(), this->getCannonAngle());
           
    newshot->displayCannonball();
    shotlist.push_back(*newshot);
            
    delete newshot;
}
Car::~Car(void) {}