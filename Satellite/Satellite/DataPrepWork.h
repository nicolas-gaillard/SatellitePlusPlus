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
	vector <vector<pair<Image, Satelite>>> timeline;
	map<Image, vector<Satelite> > matchingMap; 
	void prepWorkThread(int i,int n);

public :
	// Constructors and destructor : 
	DataPrepWork();
	DataPrepWork(SimulationData*, vector <vector<pair<Image, Satelite>>>, map<Image, vector<Satelite> >);
	DataPrepWork::DataPrepWork(SimulationData *d);
	~DataPrepWork();

	// Functions :
	vector<vector<pair<Image, Satelite>>> GetTimeLine();

	// Getters and setters :
	void setData(SimulationData*);
	SimulationData* getData();
};