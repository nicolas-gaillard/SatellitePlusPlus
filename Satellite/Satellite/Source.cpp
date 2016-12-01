#include "DataReceiver.h"
#include <iostream>

int main() {
	std::cout << "[I] Start of data reading..." << std::endl;
	DataReceiver * data_r = new DataReceiver("text/constellation.in");
	SimulationData data_s = data_r->extractData();
	std::cout << "[I] Data reading complete." << std::endl;
	std::cout << "[I] Current stats : " << std::endl;
	std::cout << "\tCollNB  \t\t" << data_s.getNbCollection() << std::endl;
	std::cout << "\tSatNB   \t\t" << data_s.getNbSatelite() << std::endl;
	std::cout << "\tDuration\t\t" << data_s.getDuration() << std::endl;
	std::cout << "\t\tSat1Pos\t" << data_s.getArraySat()[0].la << ';' << data_s.getArraySat()[0].lo << std::endl;
	system("pause");
}

