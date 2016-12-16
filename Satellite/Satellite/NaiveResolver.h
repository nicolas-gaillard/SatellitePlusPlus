#pragma once
#include <vector>
#include <string>
#include "SimulationData.h"
#include "Utils.h"
#include "SatUtils.h"

class NaiveResolver {

public:

	NaiveResolver(SimulationData * simData, std::string filename);
	~NaiveResolver();

	void launchResolution(bool verbose = true);
	void threadResolv(int i, int n,bool verbose,std::string * result);
	bool checkUncompleteCollections();
	void resetTakenPictures();

private:

	int nbPict;
	SimulationData * simData;
	Satelite * initialData;
	std::string solution;
	std::string outFilename;
};