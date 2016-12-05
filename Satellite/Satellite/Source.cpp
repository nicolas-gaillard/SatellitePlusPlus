#include "DataReceiver.h"
#include "NaiveResolver.h"
#include <iostream>
#include <string>

int main() {

	std::string filename = "text/forever_alone.in";
	std::string outfilename = "text/forever_alone.out";

	std::cout << "================================" << std::endl;
	std::cout << "\tSATELLITE PLUS PLUS" << std::endl;
	std::cout << "================================" << std::endl;
	std::cout << "[I] Starting naive resolution..." << std::endl;

	std::cout << "[I] Reading data from  '" << filename << "'... " << std::endl;
	DataReceiver * dataReceiver = new DataReceiver(filename);
	SimulationData * simData = &dataReceiver->extractData();

	if (simData->getNbSatelite() == 0)
		std::cout << "[E] Error on reading file : No such file." << std::endl;
	
	std::cout << "[S] Success." << std::endl;

	NaiveResolver * naive = new NaiveResolver(simData, outfilename);

	naive->launchResolution();

	system("pause");

	
}

