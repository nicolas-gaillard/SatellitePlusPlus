#pragma once
#include <vector>
#include <string>
#include "SimulationData.h"

class NaiveResolver {

public:

	NaiveResolver(SimulationData * simData, std::string filename);
	~NaiveResolver();

	void launchResolution(bool verbose = true);

private:

	SimulationData * simData;
	std::string solution;
	std::string outFilename;
};