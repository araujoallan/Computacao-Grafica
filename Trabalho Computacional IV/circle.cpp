#include "circle.h"

extern GLfloat window_size;
using namespace std;

Circle::Circle() {}

Point Circle::getCenter() {
	return center;
}	
float Circle::getRadius() {
	return radius;
}
string Circle::getFill() {
	return fill;
}
string Circle::getId() {
	return id;
}
void Circle::setCenter(Point center) {
	this->center = center;
}
void Circle::setRadius(float radius) {
	this->radius = radius;
}
void Circle::setFill(string fill) {
	this->fill = fill;
}
void Circle::setId(string id) {
	this->id = id;
}
void Circle::colorCheck(string color) {
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
void Circle::displayCircle(Circle *circle) {
    int triangleAmount = 70;
    float twicePi = 2.0f * PI;

    glBegin(GL_TRIANGLE_FAN);
        circle->colorCheck(circle->getFill());
        glVertex2f(circle->getCenter().x, window_size - circle->getCenter().y);
 
        for(int i = 0; i <= triangleAmount; i++)  
            glVertex2f(circle->getCenter().x + (circle->getRadius() * cos(i *  twicePi / triangleAmount)), 
                            window_size - circle->getCenter().y + (circle->getRadius() * sin(i * twicePi / triangleAmount)));
 
     glEnd();
}
Circle::~Circle(void) {}