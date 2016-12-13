#pragma once
#include <vector>
#include <string>
#include "SimulationData.h"

class NaiveResolver {

public:

	NaiveResolver(SimulationData * simData, std::string filename);
	~NaiveResolver();

	void launchResolution(bool verbose = true);
	void threadResolv(int i, int n,bool verbose,std::string * result);

private:
	int nbConflict;
	int nbPict;
	SimulationData * simData;
	std::string solution;
	std::string outFilename;
};