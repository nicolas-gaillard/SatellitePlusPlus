#include "JudgeOutput.h"
#include "DataReceiver.h"
#include "SimulationData.h"
#include <iostream>
#include <regex>


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

    // Get of the first line
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
    return true;
}

/*
 * Check if the image img was in the range of the satelite sat 
 * Return true if it was in range, false otherwise
*/
bool JudgeOutput::isValidImage(Image img, Satelite sat) {
    // À VOIR DEMAIN SUR LA RÉUNION
    // Voir si la collection image -> satellite existe
    return false;
}



/*
 * Get the score of the simulation 
 * Return the score
*/
int JudgeOutput::getScore(Collection * arrayCol) {
    int score = 0;
    // VOIR POUR LA STRUCTURE QUI RECUPERE QUELLE IMAGE A ÉTÉ PRISE PAR QUELLE(S) SATELLITE(S)

    // Parcours du tableau
    int numCol = 0;
    int numImg = 0;
    Collection col = arrayCol[numCol];;
    while (col.nbImg != 0) { 
        
        std::cout << col.nbImg << std::endl;
        for (numImg = 0; numImg <= col.nbImg; numImg++) {
            std::cout << "Col " << numCol << ", img " << numImg << " : " << col.listImg[numImg].la << " " << col.listImg[numImg].lo << std::endl;
        }
        numCol++;
        col = arrayCol[numCol];
        if (col.nbImg == 0) {
            std::cout << "mdrrr" << std::endl;
            break;
        }
    }

    return score;
}