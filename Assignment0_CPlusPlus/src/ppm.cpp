#include "PPM.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

std::vector<unsigned int> parseFile(std::ifstream& inFile) {
    std::vector<unsigned int> data;
    std::string line;
    std::string word;
    while(std::getline(inFile, line)) {
        std::istringstream iss(line);
        while(std::getline(iss, word, ' ')) {
            if (word == "#") {
                break;
            }
            try {
                unsigned int word2int = std::stoi(word);
                data.push_back(word2int);
            } catch (const std::invalid_argument& ia) {

            }
        }
    }
    return data;
}

// Constructor loads a filename with the .ppm extension
PPM::PPM(std::string fileName){
    std::ifstream inFile(fileName);
    unsigned int currPixel = 0;

    if(inFile.is_open()) {
        std::vector<unsigned int> data = parseFile(inFile); 
        for(unsigned int i = 0; i < data.size(); ++i) {
            if (i == 0) {
                std::cout << "SPAGET" << std::endl;
                std::cout << data[i] << std::endl;
                std::cout << data[i+1] << std::endl;
                m_width = data[i];
                m_height = data[i+1];
                m_PixelData = new unsigned char[3*m_width*m_height];
            }
            else if(i > 3) {
                m_PixelData[currPixel] = (char)data[i];
                currPixel++; 
            }
        }

        inFile.close();
    }
}

// Destructor clears any memory that has been allocated
PPM::~PPM(){
    delete[] m_PixelData;
}

// Saves a PPM Image to a new file.
void PPM::savePPM(std::string outputFileName){
    std::ofstream outfile;
    outfile.open(outputFileName);
    outfile << "P3" << std::endl;
    outfile << m_width << " " << m_height << std::endl;
    outfile << "255" << std::endl;
    for(unsigned int y = 0; y < m_height; ++y) {
        for(unsigned int x = 0; x < m_width; ++x) {
            unsigned int pixel = 3*(y*m_width + x);
            outfile << (unsigned int) m_PixelData[pixel] << " " << (unsigned int) m_PixelData[pixel+1] << " " << (unsigned int) m_PixelData[pixel+2] << " ";
        }
        outfile << std::endl;
    }
    outfile.close();
}

unsigned int darkenColorBy50(unsigned int c) {
    if (c < 50) {
        return 50;
    }
    else {
        return c - 50;
    }
}

// Darken subtracts 50 from each of the red, green
// and blue color components of all of the pixels
// in the PPM. Note that no values may be less than
// 0 in a ppm.
void PPM::darken(){
    for(unsigned int y = 0; y < m_height; ++y) {
        for(unsigned int x = 0; x < m_width; ++x) {
            unsigned int pixel = 3*(y*m_width + x);
            unsigned int r = (unsigned int) m_PixelData[pixel];
            unsigned int g = (unsigned int) m_PixelData[pixel+1];
            unsigned int b = (unsigned int) m_PixelData[pixel+2];
            m_PixelData[pixel] = (char) darkenColorBy50(r);
            m_PixelData[pixel+1] = (char) darkenColorBy50(g);
            m_PixelData[pixel+2] = (char) darkenColorBy50(b);
        }
    }
}

// Sets a pixel to a specific R,G,B value 
void PPM::setPixel(int x, int y, int R, int G, int B){
    unsigned int pixel = 3*(y*m_width + x);
    m_PixelData[pixel] = (char) R;
    m_PixelData[pixel+1] = (char) G;
    m_PixelData[pixel+2] = (char) B;
}
