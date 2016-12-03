#include "DataPrepWork.h"
#include <numeric>
#include <chrono>

DataPrepWork::DataPrepWork() {
	data = new SimulationData();
}


DataPrepWork::DataPrepWork(SimulationData *d, vector <vector<pair<Image, Satelite>>> m1, map<Image, vector<Satelite> > m2) {
	data = d;
	timeline = m1;
	matchingMap = m2;
}


 DataPrepWork::DataPrepWork(SimulationData *d) {
	data = d;
	timeline = vector<vector<pair<Image, Satelite>>>();
	matchingMap = map<Image, vector<Satelite> >();
}

DataPrepWork::~DataPrepWork(){}

void DataPrepWork::setData(SimulationData *d) {
	data = d;
}

SimulationData* DataPrepWork::getData() {
	return data;
}

vector <vector<pair<Image, Satelite>>> DataPrepWork::GetTimeLine() {
	
	auto start = std::chrono::high_resolution_clock::now();

	int averageSatPerImage;
	int averageImagePerSatelite;
	// Key = Simulation's turn
	//cout << getData()->getNbSatelite() << " " << getData()->getNbCollection() << endl;
	// Running the simulation :
	
	for (int t = 0; t < getData()->getDuration(); t++)
	{
		if (t % 1000 == 0)
			cout << t << endl;
		timeline.push_back(vector<pair<Image, Satelite>>());
		// For each satellite :
		for (int s = 0; s < getData()->getNbSatelite(); s++)
		{
			// Changing satellite's position :

			// We suppose that each lattiude/longitude are already in seconds (")
			long sumLaSpeed = getData()->getArraySat()[s].la + (long)getData()->getArraySat()[s].speed;

			// First case, the easiest :
			if ( (-324000 <= sumLaSpeed) && (sumLaSpeed <= 324000)){
				getData()->getArraySat()[s].la = sumLaSpeed;
				getData()->getArraySat()[s].lo -= 15;
			}

			// Second case : the satellite has reached the North Pole 
			else if (sumLaSpeed > 324000){
				getData()->getArraySat()[s].la = 648000 - (sumLaSpeed);
				getData()->getArraySat()[s].lo = -648000 + (getData()->getArraySat()[s].lo - 15);
				getData()->getArraySat()[s].speed *= (-1);
			}

			// Last case : the satellite has reached the South Pole 
			else {
				getData()->getArraySat()[s].la = -648000 - (sumLaSpeed);
				getData()->getArraySat()[s].lo = -648000 + (getData()->getArraySat()[s].lo + 15);
				getData()->getArraySat()[s].speed *= (-1);
			}

			// Satellite can't take an image above latitude (-)85° (=306000")
			if(getData()->getArraySat()[s].la >= -306000 && getData()->getArraySat()[s].la <= 306000){

				// Checking all the collection 
				for (int c = 0; c < getData()->getNbCollection(); c++)
				{

					// For each image : we check if the image is in the range of the satellite
					for (int i = 0; i < getData()->getArrayCol()[c].nbImg; i++)
					{
						long imgLa = getData()->getArrayCol()[c].listImg[i].la;
						long imgLo = getData()->getArrayCol()[c].listImg[i].lo;
						long satLa = getData()->getArraySat()[s].la;
						long satLo = getData()->getArraySat()[s].lo;
						long satDelta = (long)getData()->getArraySat()[s].maxRot;
						
						if (((imgLa >= satLa - satDelta) && (imgLa <= satLa + satDelta)) 
							&& ((imgLo >= satLo - satDelta) && (imgLo <= satLo + satDelta))){

							// We add this image to the map 
							//cout << " satelite " << s << " collide with  image  " << i << " time " << t << endl;
							timeline[t].push_back (make_pair(getData()->getArrayCol()[c].listImg[i] , getData()->getArraySat()[s]));
						}
					}		
				}
			}
		}
	}
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> diff = end - start;
	return timeline;
}