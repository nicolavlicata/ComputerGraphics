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
	
	// 2 tuples representing geometry textures 
	std::vector<float> textures;

	// 3 tuples representing normalized directional vectors
	std::vector<float> normals;
	
	// Index of each vertex in the vertices matrix. Assumption is that faces are triangles
	std::vector<int> faceVertices;
	//std::vector<std::vector<int>> faceVertices;

	// Index of each texture in the texture ppm.
	std::vector<int> faceTextures;

	//std::vector<std::vector<int>> faceVertices;
	// Index of each normal in the normals matrix
	std::vector<int> faceNormals;
	//std::vector<std::vector<int>> faceNormals;

	// All vertex information combined. each vertex defined by 5 floats x,y,z from vertices, and s,t from textures.
	std::vector<float> combinedPositionData;
	std::vector<float> combinedTextureData;

	// Indices for vertices in the vertexData. A combination of faceVertices and faceTextures.
	std::vector<int> combinedIndexes;

	// path to the ppm file containing diffuse texture information. defined by the .mtl file referenced by a .obj file's "mtllib" line.
	std::string diffuseTextureFilename;
	
	// build Obj based on an input .obj file
	Obj(std::string filename);

	// ~Obj(); // I don't think we need a destructor, because we never call "new"

	private:
		void parseVertex(std::vector<std::string> tokens);
		void parseNormal(std::vector<std::string> tokens);
		void parseTexture(std::vector<std::string> tokens);
		void parseFace(std::vector<std::string> tokens);
		void parseMtl(std::vector<std::string> tokens, std::string filename);
		void combineIndexes();
};
