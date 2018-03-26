#include "rectangle.h"
#include <string>

extern GLfloat window_size;

using namespace std;

Point Rectangle::Rectangle::getCenter() {
	return center;
}
float Rectangle::Rectangle::getWidth(){
	return width;
}
float Rectangle::Rectangle::getHeight() {
	return height;
}
string Rectangle::Rectangle::getFill() {
	return fill;
}
string Rectangle::Rectangle::getId() {
	return id;
}
void Rectangle::Rectangle::setCenter(Point center) {
	this->center = center;
}
void Rectangle::Rectangle::setWidth(float width) {
	this->width = width;
}
void Rectangle::Rectangle::setHeight(float height) {
	this->height = height;
}
void Rectangle::Rectangle::setFill(string fill) {
	this->fill  =fill;
}
void Rectangle::Rectangle::setId(string id) {
	this->id = id;
}
void Rectangle::colorCheck(string color) {
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
void Rectangle::displayRectangle(Rectangle *rect) {
	glBegin(GL_POLYGON);
        colorCheck(rect->getFill());
        glVertex2f(rect->getCenter().getX(), window_size - rect->getCenter().getY());
        glVertex2f(rect->getCenter().getX() + rect->getWidth(), window_size - (rect->getCenter().getY()));
        glVertex2f(rect->getCenter().getX() + rect->getWidth(), window_size - (rect->getCenter().getY() + rect->getHeight()));
        glVertex2f(rect->getCenter().getX(), window_size - (rect->getCenter().getY() + rect->getHeight()));
    glEnd();
}