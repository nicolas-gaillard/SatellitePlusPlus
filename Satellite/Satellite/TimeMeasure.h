#pragma once
#include <string>
#include <fstream>
#include <map>
#include <utility>			// pair
#include <cstdlib>			// rand()
#include <unistd.h>			// usleep(microsecs), sleep(seconds)
#include <chrono>			// time measures
#include <iostream>

class TimeMeasure{

private:

	std::string inputFolder;		// Folder which contains all executables				
	std::ifstream * nameOutput;		// File which will display results
	std::map<int, std::pair<long, long> > resultTabs;
	std::string inputData;		// File which contains data of 1 simulation

public:
	// Constructors and destructor :
	// -----------------------------
	TimeMeasure(std::string, std::string, std::string);
	~TimeMeasure();

	// Functions :
	// -----------
	
	// Measures the execution time of 1 executable on 1 data set :
	long measureExec(std::string pathExecutable, std::string outputExec);

	// Launch each executable :
	bool executeFolder();

	// Create the file where all the results are displayed :
	bool createResults();
};