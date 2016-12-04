#include "SimulationData.h"



SimulationData::SimulationData()
{
	this->listIntegral = std::vector<Image>();
}


SimulationData::~SimulationData()
{
}

void SimulationData::pushback(Image im) {
	this->listIntegral.push_back(im);
}