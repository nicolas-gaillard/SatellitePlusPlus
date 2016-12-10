#pragma once
#include <string>
#include <fstream>
#include <map>
#include <utility>			// pair
#include <cstdlib>			// rand()
#include <unistd.h>			// usleep(microsecs), sleep(seconds)
#include <chrono>			// time measures
#include <iostream>

#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

class TimeMeasure{

private:

	std::string inputFolder;		// Folder which contains all executables				
	std::ifstream *outputResults;	// File which displays results
	std::map<std::string, std::pair<long, long> > resultTabs;
	std::string inputData;			// Data set of one simulation

public:
	// Constructors and destructor :
	// -----------------------------
	TimeMeasure(std::string, std::string, std::string);
	~TimeMeasure();

	// Functions :
	// -----------
	
	// Measures the execution time of 1 executable on 1 data set :
	long measureExec(std::string pathExecutable, std::string outputExec);

	// List all files in a directory
	bool getFilesInDirectory(std::vector<std::string> &out);

	// Launch each executable :
	bool executeFolder();
	// search : browse directory cpp/c++

	// Create the file where all the results are displayed :
	bool createResults();
};