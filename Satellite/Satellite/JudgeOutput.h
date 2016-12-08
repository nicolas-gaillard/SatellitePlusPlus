#pragma once
#include "DataReceiver.h"
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

class JudgeOutput {

public:
	JudgeOutput(std::string filename);
	//~JudgeOutput();

	bool isValidOutput();
    bool isValidFormat();
    bool isValidImage(Image img, Satelite sat);
    int getScore(Collection * arrayCol, long nbCol);
	std::vector<Image> getImagesTaken();

private:
	std::ifstream * outputFile;
    
};