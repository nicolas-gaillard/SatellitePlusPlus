#include "JudgeOutput.h"
#include <iostream>
#include <regex>

void splitStr(const std::string &s, char delim, std::vector<std::string> &elems) {
	std::stringstream ss;
	ss.str(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
}

/*
 * Check if images contains img
 * Return true if, false otherwise
 */
bool contains(std::vector<Image> &images, Image img) {
    std::vector<Image>::iterator it;
    for(it=images.begin() ; it < images.end(); it++) {
        if (((*it).la == img.la)&&((*it).lo == img.lo)) {
            return true;
        }
    }
    return false;
}

/*
 * Contructor 
 * filename : path of the output file
*/
JudgeOutput::JudgeOutput(std::string outFileName) {
    outputFile = new std::ifstream(outFileName);
}

/*
 * Check if the output file is valid :
    * The format of the output file matches the description.
    * Each picture is in range of the corresponding satellite when it’s taken.
    * No satellite moves the camera faster than w arcseconds per turn between taking two consecutive pictures.
 * Return true if valid, false otherwise
*/
bool JudgeOutput::isValidOutput() {
    return isValidFormat();
}

/* 
 * Check if the file format is valid
 * Return true if valid, false otherwise
*/
bool JudgeOutput::isValidFormat() {
    // CHECK THE SYNTAX : regex
    // First line : only a digit (number of pictures taken)
    std::regex first_line("[[:digit:]]+");
    // The other lines : image i taken by satellite s at turn t
    std::regex line_regex("(-?[[:digit:]]+[[:space:]]){2}([[:digit:]]+[[:space:]])[[:digit:]]+");

    // Get the first line
	std::string line;
	std::getline(*outputFile, line);

    // Check the first line
    if (!regex_match(line, first_line)) {
        std::cout << "[E] Test failed at line : '" << line << "'." << std::endl;
        return false;
    }

    // Check the other lines
    while (std::getline(*outputFile, line)) {
        if (!regex_match(line, line_regex)) {
            std::cout << "[E] Test failed at line : '" << line << "'." << std::endl;
            return false;
        }
    }
    // Back to the beginning of the file
    outputFile->clear();
    outputFile->seekg(0, std::ios::beg);
    return true;
}



/*
 * Check if the image img was in the range of the satelite sat 
 * Return true if it was in range, false otherwise
*/
bool JudgeOutput::isValidImage(Image img, Satelite sat) {
    // attendre la fonction d'étienne de calcul de position, d'un satellite
    // faire à partir du fichier de sortie
    return false;
}

/*
 * Get all the images taken from the outputFile
 * Return a vector of all the images
*/
std::vector<Image> JudgeOutput::getImagesTaken() {
    std::vector<Image> imgs;
    Image * img;
    std::vector<std::string> elems;
	std::string line;

    // Get the first line : number of image taken
	std::getline(*outputFile, line);
	int nbImageTaken = std::stoi(line);

    // Browse all the images 
    for (int i = 0; i < nbImageTaken; i++) {
        img = new Image();

        std::getline(*outputFile, line);
        splitStr(line, ' ', elems);

        img->la = std::stoi(elems[0]);
        img->lo = std::stoi(elems[1]);

        imgs.push_back(*img);
        elems.clear();
    }
    return imgs;
}

/*
 * Get the score of the simulation 
 * Return the score
*/
int JudgeOutput::getScore(Collection * arrayCol, long nbCol) {
    int score = 0;
    
    int numCol;
    int numImg;
    bool colComplete;
    Image img;

    std::vector<Image> images = this->getImagesTaken();

    //  Browse the collection array 
    for (numCol = 0; numCol < nbCol; numCol++) { 
        Collection col = arrayCol[numCol];
        colComplete = true;
        std::cout << "Nb img : " << col.nbImg << std::endl;
        // Browse pictures of collection col
        for (numImg = 0; numImg < col.nbImg; numImg++) {
            img = col.listImg[numImg];
            std::cout << "Col " << numCol << ", img " << numImg << " : " << img.la << " " << img.lo;
           
            // Check if img was taken
            if (!contains(images, img)) {
                colComplete = false;
                std::cout << std::endl;
            }
            else  {
                std::cout << " _/" << std::endl;
            }
        }
        // If col complete : add the score of col
        if (colComplete) {
            score += col.nbPts;
            std::cout << "Collection " << numCol << " complete" << std::endl;
        }
    }
    return score;
}