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

#include "JudgeOutput.h"

class TimeMeasure{

private:

	std::string inputFolder;		// Folder which contains all executables				
	std::string inputData;			// Data set of one simulation
	std::fstream outputResults;		// File which displays results
	std::map<std::string, std::pair<long, long> > resultTabs;
	/*
	 * Stores the execution time and the score of each executable
	 *		- 1st long : execution time
	 *		- 2nd long : score
	 */ 

public:
	// Constructors and destructor :
	// -----------------------------
	TimeMeasure(std::string, std::string, std::string);
	TimeMeasure(std::string);
	~TimeMeasure();

	// Getters 
	// -------

	inline std::fstream& getOutputResults(){return outputResults;}
	inline std::map<std::string, std::pair<long, long> > getResultTabs(){return resultTabs;}
	inline std::string getInputData(){return inputData;}
	inline std::string getInputFolder(){return inputFolder;}
	
	// Setters :
	// ---------
	inline void setInputData(std::string s) {inputData = s;}
	inline void setInputFolder(std::string s) {inputFolder = s;}
	inline void setOutputResults(std::string s) {outputResults = std::fstream(s.c_str(), std::fstream::in | std::fstream::out | std::fstream::trunc);;}

	// Functions :
	// -----------
	
	// Measures the execution time of 1 executable on 1 data set :
	long measureExec(std::string pathExecutable, std::string outputExec);
	long measureExec(std::string);

	// Lists all files in a directory
	bool getFilesInDirectory(std::vector<std::string> &out);

	// Launches each executable :
	bool executeFolder();
	// search : browse directory cpp/c++

	// Creates the file where all the results are displayed :
	bool createResults();
	// search : ascii art c++
};