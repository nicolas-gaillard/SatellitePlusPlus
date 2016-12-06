#include "JudgeOutput.h"
#include <iostream>
#include <regex>


/*
 * Contructor 
 * filename : path of the output file
*/
JudgeOutput::JudgeOutput(std::string filename) {
    outputFile = new std::ifstream(filename);
}

/*
 * Check if the output file is valid 
 * Return true if valid, false otherwise
*/
bool JudgeOutput::isValidOutput() {
    return false;
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
    return false;
}


/*
 * Get the score of the simulation 
 * Return the score
*/
int getScore() {
    return 0;
}