 #include "Judge.h"
#include <regex>
#include <sstream>
#include <vector>
#include <iostream>
#include "DataReceiver.h"

void splitString(const std::string &s, char delim, std::vector<std::string> &elems) {
	std::stringstream ss;
	ss.str(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
}

Judge::Judge(std::string filename) {
	inputFile = new std::ifstream(filename);
}

bool Judge::isValidSyntax(int maxPictures, int maxTurns, int satNb) {

	// ----- SYNTAX CHECK -----

	// Definition of usefull regular expressions.
	std::regex number_regex("[[:digit:]]+");
	std::regex line_regex("-?([[:digit:]]+[[:space:]]){3}[[:digit:]]+");

	// Get of the first line
	std::string line;
	std::getline(*inputFile, line);

	// Test of the first line
	if (!std::regex_match(line, number_regex)) {
		// Test failed.
		std::cout << "[E] Test failed at line : '" << line << "'." << std::endl;
		return false;
	}

	// Convert number after check pass
	int picturesNb = std::stoi(line);

	// Creation of the picture array
	std::string * pictureArray = new std::string[picturesNb];

	// Test of each line
	int iter = 0;
	while (std::getline(*inputFile, line)) {
		if (!std::regex_match(line, line_regex)) {
			// Test failed.
			std::cout << "[E] Test failed at line : '" << line << "'." << std::endl;
			return false;
		}
		else {
			// Test success : line saved.
			pictureArray[iter] = line;
			iter++;
		}
	}


	// ------- SEMANTICAL CHECK --------

	// Test of the number of pictures
	if (picturesNb > maxPictures || picturesNb <= 0) {
		return false;
	}

	std::vector<std::string> elems;
	int la, lo, tu, sa;

	// Test of each line
	for (int i = 0; i < picturesNb; i++)
	{
		splitString(pictureArray[i], ' ', elems);
		try
		{
			la = std::stoi(elems[0]);
			lo = std::stoi(elems[1]);
			tu = std::stoi(elems[2]);
			sa = std::stoi(elems[3]);
		}
		catch (const std::exception&)
		{
			return false;
		}
		if ((la < -306000) || (la > 306000) || (lo < -648000) || (lo > 647999) || (tu > maxTurns) || (tu < 0) || (sa+1 > satNb) || (sa < 0)) {
			return false;
		}
	}
	return true;
}
