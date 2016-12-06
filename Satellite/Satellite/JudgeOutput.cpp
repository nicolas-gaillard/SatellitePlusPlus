#include "Judge.h"
#include <iostream>


/*
 * Contrucotr 
 * filename : path of the output file
*/
JudgeOutput::JudgeOutput(std::string filename) {
    outputFile = new std::ifstream(filename);
}



