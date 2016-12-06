#include "DataReceiver.h"
#include "DataPrepWork.h"

int main() {

	DataReceiver  rc =  DataReceiver("text/overlap.IN");
	SimulationData data = rc.extractData();
	cout << data.getNbCollection() << " " << data.getNbSatelite() << endl;
	cout << "nombre d'image " << data.getListIntegral().size();
	DataPrepWork prep = DataPrepWork(&data);
	prep.GetTimeLine();
	getchar();
	return 0;
}

