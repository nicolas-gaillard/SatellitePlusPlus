#pragma once
#include "SimulationData.h"
#include <string>
#include <fstream>
#include "StructData.h"
#include <sstream>
#include <vector>
#include <iostream>
#include <algorithm>

class DataReceiver
{
public:
	DataReceiver(std::string filename,int percent);
	~DataReceiver();
	SimulationData extractData();
	Satelite * extractSatelite(SimulationData * Sd);
	Collection * extractCollection(SimulationData * Sd);
	Image * extractImage(int nb);
	TimeStamp * extractTimeStamp(int nb);

	// Remove a percentage of collection (according to its score and its number of points) 
	Collection * optiExtractCollection(SimulationData * Sd, int threshold);
	// (Threshold in percent)

private:
	std::ifstream * infile;
	int percent;


};
