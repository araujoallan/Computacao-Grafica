#include "arena.h"

string Arena::getName() {
	return name;
}
string Arena::getType() {
	return type;
}
string Arena::getSVGPath() {
	return svgFile_path;
}
void Arena::setName(string name) {
	this->name = name;
}
void Arena::setType(string type) {
	this->type = type;
}
void Arena::setSVGPath(string svgPath) {
	svgFile_path = svgPath;
}