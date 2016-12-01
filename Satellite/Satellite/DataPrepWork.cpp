#include "DataPrepWork.h"

DataPrepWork::DataPrepWork() {
	data = new SimulationData();
}

DataPrepWork::DataPrepWork(SimulationData *d, map<long, pair<Image, Satelite> > * m1, map<Image, vector<Satelite> > m2) {
	data = d;
	timeline = m1;
	matchingMap = m2;
}

DataPrepWork::~DataPrepWork(){

}

void DataPrepWork::setData(SimulationData *d) {
	data = d;
}


SimulationData* DataPrepWork::getData() {
	return data;
}

map<long, pair<Image, Satelite> > DataPrepWork::GetTimeLine() {
	map<long, pair<Image, Satelite> > result;

	// Numbers of image which can potientally be taken during this simulation
	long count = 0;

	// Running the simulation
	for (int t = 0; t < getData()->getDuration(); t++)
	{
		// For each satellite
		for (int s = 0; s < getData()->getNbSatelite(); s++)
		{
			// Changing satellite's position :

			// We suppose that each lattiude/longitude are already in seconds (")
			long sumLaSpeed = getData()->getArraySat()[s].la + (long)getData()->getArraySat()[s].speed;

			if ( (-324000 <= sumLaSpeed) and (sumLaSpeed <= 324000)){
				getData()->getArraySat()[s].la = sumLaSpeed;
				getData()->getArraySat()[s].lo -= 15;
			}

			else if (sumLaSpeed > 324000){
				getData()->getArraySat()[s].la = 648000 - (sumLaSpeed);
				getData()->getArraySat()[s].lo = -648000 + (getData()->getArraySat()[s].lo - 15);
				getData()->getArraySat()[s].speed *= (-1);
			}

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
						
						if (((imgLa >= satLa - satDelta) and (imgLa <= satLa + satDelta)) 
							and ((imgLo >= satLo - satDelta) and (imgLo <= satLo + satDelta))){

							result[count++] = make_pair(getData()->getArrayCol()[c].listImg[i] , getData()->getArraySat()[s]);
						}
					}
					
				}
			}

			
		}
	}
	

	return result;
}