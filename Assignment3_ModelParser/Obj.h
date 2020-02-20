#pragma once

#include <vector>
#include <iostream>

#include <fstream>
#include <sstream>

// Functions for parsing .obj files.
class Obj {
	public:
	// 3 tuples representing geometry vertices
	std::vector<float> vertices;
	
	// 3 tuples representing normalized directional vectors
	std::vector<float> normals;
	
	// Index of each vertex in the vertices matrix. Assumption is that faces are triangles
	std::vector<int> faceVertices;
	//std::vector<std::vector<int>> faceVertices;

	// Index of each normal in the normals matrix
	std::vector<int> faceNormals;
	//std::vector<std::vector<int>> faceNormals;

	// build Obj based on an input .obj file
	Obj(std::string filename);

	// cleanup allocated data structures.
	~Obj();

	private:
		void parseVertex(std::vector<std::string> tokens);
		void parseNormal(std::vector<std::string> tokens);
		void parseFace(std::vector<std::string> tokens);
};
