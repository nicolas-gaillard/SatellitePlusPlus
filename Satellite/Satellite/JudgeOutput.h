#pragma once
#include "DataReceiver.h"
#include "SatUtils.h"
#include "Utils.h"
#include <algorithm>

class JudgeOutput {

public:
	JudgeOutput(std::string filename, SimulationData * simData);
	//~JudgeOutput();

	bool isValidOutput();
	bool isValidImages();
    bool isValidSyntax();
    bool isValidImage(int turn, Image * img, Satelite * sat);
	bool isValidCamera();
    int getScore();
	std::vector<Image> getImagesTaken();

private:
	std::ifstream * outputFile;
	SimulationData * simData;
    
};