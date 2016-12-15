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
	DataReceiver(std::string filename);
	~DataReceiver();
	SimulationData extractData();
	Satelite * extractSatelite(SimulationData * Sd);
	Collection * extractCollection(SimulationData * Sd);
	Image * extractImage(int nb);
	TimeStamp * extractTimeStamp(int nb);

	Collection * optiExtractCollection(SimulationData * Sd, int threshold);

private:
	std::ifstream * infile;

};
