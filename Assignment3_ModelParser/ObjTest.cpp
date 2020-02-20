#include "Obj.h"
#include <stdio.h>

int main() {
	std::cout << "Beginning ObjTest" << std::endl;
	Obj myObj("./objects/cube.obj");
	std::cout << myObj.vertices[0] << std::endl;
	std::cout << "End ObjTest" << std::endl;
	return 0;
}
