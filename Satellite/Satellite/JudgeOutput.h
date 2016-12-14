#pragma once
#include "DataReceiver.h"
#include <algorithm>

class JudgeOutput {

public:
	JudgeOutput(std::string filename, SimulationData * simData);
	//~JudgeOutput();

	bool isValidOutput();
	bool isValidImages();
    bool isValidFormat();
    bool isValidImage(int turn, Image * img, Satelite * sat);
	bool isValidCamera();
    int getScore();
	std::vector<Image> getImagesTaken();

private:
	std::ifstream * outputFile;
	SimulationData * simData;
    
};