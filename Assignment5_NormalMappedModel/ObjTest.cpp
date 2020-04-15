#include "Obj.h"
#include <stdio.h>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>

void vectorToFile(std::vector<float> v, std::string filename) {
	std::ofstream output_file(filename);
    std::ostream_iterator<float> output_iterator(output_file, "\n");
    std::copy(v.begin(), v.end(), output_iterator);
}

void vectorToFile(std::vector<int> v, std::string filename) {
	std::ofstream output_file(filename);
    std::ostream_iterator<int> output_iterator(output_file, "\n");
    std::copy(v.begin(), v.end(), output_iterator);
}

int main() {
	std::cout << "Beginning ObjTest" << std::endl;
	Obj myObj("./objects/house/house_obj.obj");
	std::cout << myObj.vertices[0] << std::endl;
	std::cout << myObj.diffuseTextureFilename << std::endl;
	std::cout << myObj.combinedIndexes.size() << std::endl;
	std::cout << myObj.combinedPositionData.size() << std::endl;
	std::cout << myObj.combinedTextureData.size() << std::endl;

	vectorToFile(myObj.faceVertices, "faceVertices.txt");
	vectorToFile(myObj.faceTextures, "faceTextures.txt");
	vectorToFile(myObj.combinedTextureData, "combinedTextureData.txt");
	vectorToFile(myObj.combinedPositionData, "combinedPositionData.txt");
	vectorToFile(myObj.combinedIndexes, "combinedIndexes.txt");

	std::cout << "End ObjTest" << std::endl;
	return 0;
}
