#pragma once
#include <string>
#include <fstream>

class JudgeOutput {

public:
	JudgeOutput(std::string filename);
	~JudgeOutput();

	bool isValidOutput();
    int getScore();
    bool isValidImage(Image img, Satelite sat);

private:
	std::ifstream * outputFile;
};