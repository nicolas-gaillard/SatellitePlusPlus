#pragma once
#include "DataReceiver.h"
#include <algorithm>

class JudgeOutput {

public:
	JudgeOutput(std::string filename);
	//~JudgeOutput();

	bool isValidOutput(Satelite * arraySat, long nbSatelite);
	bool isValidImages(Satelite * arraySat, long nbSatelite);
    bool isValidFormat();
    bool isValidImage(int turn, Image * img, Satelite * sat);
	bool isValidCamera();
    int getScore(Collection * arrayCol, long nbCol);
	std::vector<Image> getImagesTaken();

private:
	std::ifstream * outputFile;
    
};