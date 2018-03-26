#ifndef ARENA_H_
#define ARENA_H_
#include <string>

using namespace std;

class Arena {
    public: string name, type, svgFile_path;

	public:
		string getName();
		string getType();
		string getSVGPath();
		void setName(string name);
		void setType(string type);
		void setSVGPath(string svgPath);
};

#endif /* ARENA_H_ */