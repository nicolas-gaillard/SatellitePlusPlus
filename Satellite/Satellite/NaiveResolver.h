#pragma once
#include <vector>
#include <string>
#include <set>
#include "DataReceiver.h"
#include "SimulationData.h"

class NaiveResolver {

public:

	NaiveResolver(SimulationData * simData, std::string filename);
	~NaiveResolver();
	
	void launchResolution(bool verbose = true);
	void threadPrepResolv(int i, int n,bool verbose,std::string * result);
	void threadResolv(int i, int n, bool verbose, std::string * result);
	void threadResolv(int i, int n, bool verbose);

private:
	void checkDoable();
	Image * makeChoice(std::vector<Image*> possible, Satelite*);
	void afterChoice(std::vector<Image*> possible,Satelite *sat,int satidx,int turn, std::string * result,Image * im);
	void resetSat();
	int nbConflict;
	int nbPict;
	std::vector<Result> res;
	std::string * resultStr;
	bool currentlyProcessing;
	int wrongcoll;
	Satelite *  initialData;
	Collection * initialCollection;
	std::vector<Collection*> available;
	SimulationData * simData;
	std::set<int> notdoable;
	std::string solution;
	std::string outFilename;
};