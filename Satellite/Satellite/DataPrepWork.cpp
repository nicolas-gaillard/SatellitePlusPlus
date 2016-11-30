#include "DataPrepWork.h"

DataPrepWork::DataPrepWork() {
	data = new SimulationData();
}

DataPrepWork::DataPrepWork(SimulationData d, map<long, pair<Image, Satelite> > * m1, map<Image, vector<Satelite> > m2) {
	this.data = d;
	this.timeline = m1;
	this.matchingMap = m2;
}

DataPrepWork::~DataPrepWork{

}

void DataPrepWork::setData(SimulationData d) {
	this.data = d;
}


SimulationData DataPrepWork::getData() {
	return this.data;
}

map<long, pair<Image, Satelite> > DataPrepWork::GetTimeLine() {
	map<long, pair<Image, Satelite> > result;

	// Running the simulation
	for (int t = 0; t < this.getData().getDuration(); t++)
	{
		// For each satellite
		for (int s = 0; i < this.getData().getNbSatelite(); s++)
		{
			// Changing satellite's position

			// Checking all the collection 
			for (int i = 0; i < this.getData().getNbCollection(); i++)
			{
				// For each image : we check if the image is in the range of the satellite
			}
		}
	}
	

	return result;
}