#ifndef READER_H_
#define READER_H_
#include "arena.h"
#include "circle.h"
#include "rectangle.h"
#include "car.h"
#include "cannonball.h"
#include <vector> 

using namespace std;

class Reader {
	public:	
		Reader();
		void loadArenaScenario(const char *filePath, Arena *arena, Circle *out_circ, Circle *in_circ, Car *car,
								Rectangle *start, vector<Car*> &enemies);
		void readFile(const char *filePath, Arena *arena, Circle *out_circ, Circle *in_circ, Car *car, Rectangle *start,
								vector<Car*> &enemies);
};

#endif /* READER_H_ */

