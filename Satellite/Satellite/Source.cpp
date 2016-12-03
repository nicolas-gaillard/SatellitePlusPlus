#include "DataReceiver.h"
#include "DataPrepWork.h"

int main() {

	DataReceiver  rc =  DataReceiver("text/constellation.IN");
	SimulationData data = rc.extractData();
	DataPrepWork prep = DataPrepWork(&data);
	return 0;
}

