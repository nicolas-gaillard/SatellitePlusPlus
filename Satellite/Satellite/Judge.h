#pragma once
#include <string>
#include <fstream>

class Judge {

public:
	Judge(std::string filename);
	~Judge();

	bool isValidSyntax(int maxPictures, int maxTurns, int satNb);

private:
	std::ifstream * inputFile;
};