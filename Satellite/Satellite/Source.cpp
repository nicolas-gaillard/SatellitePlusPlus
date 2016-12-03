#include "DataReceiver.h"
#include "DataPrepWork.h"

int main() {

	DataReceiver  rc =  DataReceiver("text/constellation.IN");
	SimulationData data = rc.extractData();
	cout << data.getNbCollection() << " " << data.getNbSatelite() << endl;
	DataPrepWork prep = DataPrepWork(&data);
	prep.GetTimeLine();
	getchar();
	return 0;
}

