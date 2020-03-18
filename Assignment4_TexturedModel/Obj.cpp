#include "Obj.h"
#include <stdio.h>
#include <string.h>
#include <map>
#include <cstdlib>

std::vector<std::string> tokenize(std::string s, std::string delimiter) {
	std::vector<std::string> tokens;
	size_t last = 0; 
	size_t next = 0;
	while ((next = s.find(delimiter, last)) != std::string::npos) { 
		tokens.push_back(s.substr(last, next - last));
		last = next + 1;
	}
	tokens.push_back(s.substr(last, s.length()-1)); // ignore the newline
	return tokens;
}

Obj::Obj(std::string filename) {
	this->diffuseTextureFilename = "";

    std::ifstream inFile(filename);
    if(inFile.is_open()) {
        std::string currentLine;
        while(std::getline(inFile, currentLine)) {
			std::vector<std::string> tokens = tokenize(currentLine, " ");
			if (!tokens[0].compare("v"))
				parseVertex(tokens);
			else if (!tokens[0].compare("vn"))
				parseNormal(tokens);
			else if (!tokens[0].compare("vt"))
				parseTexture(tokens);
			else if (!tokens[0].compare("f"))
				parseFace(tokens);
			else if (!tokens[0].compare("mtllib"))
				parseMtl(tokens, filename);
		}
    }

	// Only have to combine if some textures were declared.
	if (this->textures.size()) {
        this->combineIndexes();
	}
    inFile.close();
}

void Obj::parseVertex(std::vector<std::string> tokens) {
	for(int i = 1; i < tokens.size(); i++) {
		vertices.push_back(strtof(tokens[i].c_str(), 0));
	}
}

void Obj::parseNormal(std::vector<std::string> tokens) {
	for(int i = 1; i < tokens.size(); i++) {
		normals.push_back(strtof(tokens[i].c_str(), 0));
	}
}

void Obj::parseTexture(std::vector<std::string> tokens) {
	for(int i = 1; i < tokens.size(); i++) {
		textures.push_back(strtof(tokens[i].c_str(), 0));
	}
}

void print(std::string msg) {
	std::cout << msg << std::endl;
}

void Obj::parseFace(std::vector<std::string> tokens) {
	//std::vector<int> verts;
	//std::vector<int> norms;
	for(int i = 1; i < tokens.size(); i++) {
		int firstDelimIndex = tokens[i].find("/", 0);
		std::string vert = tokens[i].substr(0, firstDelimIndex);
		std::string afterVert = tokens[i].substr(firstDelimIndex+1);
		int secondDelimIndex = afterVert.find("/", 0);
		std::string texture = afterVert.substr(0, secondDelimIndex); 
		std::string norm = afterVert.substr(secondDelimIndex+1); 
		//verts.push_back(stoi(vert)-1);
		//norms.push_back(stoi(norm)-1);
		faceVertices.push_back(stoi(vert)-1);
	 	try {
	 	  faceTextures.push_back(stoi(texture)-1);
	 	} catch (const std::invalid_argument& ia) {
	 	  // ignore
	 	}
		faceNormals.push_back(stoi(norm)-1);
	}
	//faceVertices.push_back(verts);
	//faceNormals.push_back(norms);
}

std::string replaceFilename(std::string path, std::string filename) {
	std::string prefix = path.substr(0, path.rfind('/')+1);
	return prefix + filename;
}

void Obj::combineIndexes() {
	int indexNumber = 0;
	std::map<std::vector<float>, int> hashMap;
	for(int i = 0; i < this->faceVertices.size(); i++) {
        int vertexIndex = faceVertices[i];
        int textureIndex = faceTextures[i];

		std::vector<float> data;
		data.push_back(vertices[(vertexIndex*3)]);
	    data.push_back(vertices[(vertexIndex*3)+1]);
	    data.push_back(vertices[(vertexIndex*3)+2]);
        data.push_back(textures[(textureIndex*2)]);
        data.push_back(textures[(textureIndex*2)+1]);

        if (hashMap.count(data)) {
            combinedIndexes.push_back(hashMap[data]);
		} else {
            hashMap[data] = indexNumber;
            combinedPositionData.push_back(data[0]);
            combinedPositionData.push_back(data[1]);
            combinedPositionData.push_back(data[2]);
            combinedTextureData.push_back(data[3]);
            combinedTextureData.push_back(data[4]);
            combinedIndexes.push_back(indexNumber);
            indexNumber++;
		}
	}
}

void Obj::parseMtl(std::vector<std::string> tokens, std::string filename) {
    // always assume the mtllib line is "mtllib filename" 
	std::string inFilename = replaceFilename(filename, tokens[1]);
    std::ifstream inFile(inFilename);
    if(inFile.is_open()) {
        std::string currentLine;
        while(std::getline(inFile, currentLine)) {
			std::vector<std::string> tok = tokenize(currentLine, " ");
			if (!tok[0].compare("map_Kd"))
				this->diffuseTextureFilename = replaceFilename(filename, tok[1]);
		}
    }
}

