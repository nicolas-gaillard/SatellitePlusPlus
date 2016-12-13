#include "DataReceiver.h"
#include "NaiveResolver.h"
#include <iostream>
#include <string>

int main() {



	std::string filename = "text/constellation.in";
	std::string outfilename = "text/constellation.out";

	std::cout << "================================" << std::endl;
	std::cout << "\tSATELLITE PLUS PLUS" << std::endl;
	std::cout << "================================" << std::endl;
	std::cout << "[I] Starting naive resolution..." << std::endl;

	std::cout << "[I] Reading data from  '" << filename << "'... " << std::endl;
	DataReceiver * dataReceiver = new DataReceiver(filename);
    SimulationData d=dataReceiver->extractData();
	SimulationData * simData = &d;

	if (simData->getNbSatelite() == 0)
		std::cout << "[E] Error on reading file : No such file." << std::endl;
	
	std::cout << "[S] Success." << std::endl;

	NaiveResolver * naive = new NaiveResolver(simData, outfilename);

	naive->launchResolution(false);

	system("pause");

	
}

