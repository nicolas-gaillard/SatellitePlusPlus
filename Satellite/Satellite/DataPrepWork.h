#pragma once

#include "SimulationData.h"
#include <map>
#include <vector> 
#include <utility>
#include <iostream>
//#include <string>

using namespace std;

class DataPrepWork {

private :
	SimulationData *data;
	map<long, pair<Image, Satelite> > *timeline;
	map<Image, vector<Satelite> > matchingMap; 

public :
	// Constructors and destructor : 
	DataPrepWork();
	DataPrepWork(SimulationData*, map<long, pair<Image, Satelite> > *, map<Image, vector<Satelite> >);
	~DataPrepWork();

	// Functions :
	map<long, pair<Image, Satelite> > GetTimeLine();

	// Getters and setters :
	void setData(SimulationData*);
	SimulationData* getData();
};