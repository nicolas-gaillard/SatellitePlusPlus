#pragma once
#include <string>
#include <fstream>
#include "StructData.h"

class JudgeOutput {

public:
	JudgeOutput(std::string filename);
	//~JudgeOutput();

	bool isValidOutput();
    bool isValidFormat();
    bool isValidImage(Image img, Satelite sat);
    int getScore(Collection * arrayCol);

private:
	std::ifstream * outputFile;
    
};