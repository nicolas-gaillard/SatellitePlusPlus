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
    std::regex first_line("[[:digit:]]+");
    std::regex line_regex("-?([[:digit:]]+[[:space:]]){2}+?([[:digit:]]+[[:space:]]){2}");
    
    std::cout << regex_match("175958 8387 12 1", line_regex) << std::endl;
    return false;
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