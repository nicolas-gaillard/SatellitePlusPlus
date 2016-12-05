#include "DataPrepWork.h"
#include <numeric>
#include <chrono>
#include <thread> 

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

void DataPrepWork::prepWorkThread(int i,int n) {
	Satelite sat;
	vector<Image> vect = getData()->getListIntegral();
	for (size_t x = i; x < this->data->getNbSatelite(); x=x+n)
	{
		auto start = std::chrono::high_resolution_clock::now();

		//cout << " satelite  " << x << " thread no " << i << "/"<<n <<endl;
		sat = this->data->getArraySat()[x];
		for (int t = 0; t < getData()->getDuration(); t++)
		{
			long sumLaSpeed = sat.la + (long)sat.speed;
			// First case, the easiest :
			if ((-324000 <= sumLaSpeed) && (sumLaSpeed <= 324000)) {
				sat.la = sumLaSpeed;
				sat.lo -= 15;
			}

			// Second case : the satellite has reached the North Pole 
			else if (sumLaSpeed > 324000) {
				sat.la = 648000 - (sumLaSpeed);
				sat.lo = -648000 + (sat.lo - 15);
				sat.speed *= (-1);
			}

			// Last case : the satellite has reached the South Pole 
			else {
				sat.la = -648000 - (sumLaSpeed);
				sat.lo = -648000 + (sat.lo + 15);
				sat.speed *= (-1);
			}

			// Satellite can't take an image above latitude (-)85° (=306000")
			if (sat.la >= -306000 && sat.la <= 306000) {
				//cout << getData()->getListIntegral().size() << endl;
				// Checking all the collection 
				
				for (int c = 0; c < getData()->getListIntegral().size(); c++)
				{


					long imgLa = vect[c].la;
					long imgLo = vect[c].lo;
					long satLa = sat.la;
					long satLo = sat.lo;
					long satDelta = (long)sat.maxRot;
					if (((imgLa >= satLa - satDelta) && (imgLa <= satLa + satDelta))
						&& ((imgLo >= satLo - satDelta) && (imgLo <= satLo + satDelta))) {
						// We add this image to the map 
						//cout << " satelite " << s << " collide with  image  " << i << " time " << t << endl;
						timeline[t].push_back(make_pair(vect[c], sat));
					}


				}
				
				/*
				for (size_t c = 0; c < data->getNbCollection(); c++)
				{
					for (int i = 0; i < getData()->getArrayCol()[c].nbImg; i++)
					{
						long imgLa = getData()->getArrayCol()[c].listImg[i].la;
						long imgLo = getData()->getArrayCol()[c].listImg[i].lo;
						long satLa = sat.la;
						long satLo = sat.lo;
						long satDelta = (long)sat.maxRot;
						if (((imgLa >= satLa - satDelta) && (imgLa <= satLa + satDelta))
							&& ((imgLo >= satLo - satDelta) && (imgLo <= satLo + satDelta))) {
							// We add this image to the map
							//cout << " satelite " << s << " collide with  image  " << i << " time " << t << endl;
							timeline[t].push_back(make_pair(vect[c], sat));
						}

					}

				}
				*/
				// For each image : we check if the image is in the range of the satellite
				

			}


		}
		auto end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> diff = end - start;
		std::cout << "Temps pour trier les mots " << diff.count() << " s\n";

	}
	
	


}


vector <vector<pair<Image, Satelite>>> DataPrepWork::GetTimeLine() {
	
	thread *t = new thread[data->getNbSatelite()];
	int nbsatelite = getData()->getNbSatelite();
	DataPrepWork * th = this;
	int tmp = 1;
	int averageSatPerImage;
	int averageImagePerSatelite;
	// Key = Simulation's turn
	// Running the simulation :

	for (int t = 0; t < getData()->getDuration(); t++)
	{
		timeline.push_back(vector<pair<Image, Satelite>>());

	}
	for (int s = 0; s < nbsatelite; s++)
	{
		// Changing satellite's position :

		
		for (int i = 0; i < tmp; i++) {
			Satelite sat = data->getArraySat()[i];
			t[i] = thread([&th,i,tmp ]() { th->prepWorkThread(i,tmp); });
		}

		for (int i = 0; i < tmp; i++) {
			t[i].join();
		}
		// We suppose that each lattiude/longitude are already in seconds (")
		//cout << "all gucci " << endl;
	}

	
	return timeline;
}