#pragma once
#include "DataReceiver.h"
#include "SatUtils.h"
#include "Utils.h"
#include <algorithm>
#include <cmath>

class JudgeOutput {

public:
	JudgeOutput(std::string filename, SimulationData * simData);
	//~JudgeOutput();

	bool isValidOutput();
	bool isValidImages();
    bool isValidSyntax();
    bool isValidImage(int turn, Image * img, Satelite * sat);
	bool isValidCamera();
	bool checkCamera(Image * lastPos, Image * img, int turn1, int turn2, Satelite* sat);
    int getScore();
	std::vector<Image> getImagesTaken();
	void goBackToBeginningOutput();

private:
	std::ifstream * outputFile;
	SimulationData * simData;
    
};
