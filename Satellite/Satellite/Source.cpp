#include "DataReceiver.h"
#include "NaiveResolver.h"
#include <iostream>
#include <string>

int main() {



	std::string filename = "text/constellation.in";

	std::cout << "================================" << std::endl;
	std::cout << "\tSATELLITE PLUS PLUS" << std::endl;
	std::cout << "================================" << std::endl;
	std::cout << "[I] Starting naive resolution..." << std::endl;

	std::cout << "[I] Reading data from  '" << filename << "'... " << std::endl;
	
		std::string outfilename = "text/weekendtestwhithoutseuil.out";
		DataReceiver * dataReceiver = new DataReceiver(filename,10,120);
		SimulationData d = dataReceiver->extractData();
		SimulationData * simData = &d;
		for (size_t  i = 0; i < simData->getNbCollection(); i++)
		{
			std::cout << "nb points" <<simData->getArrayCol()[i].nbPts << "nb image "<< simData->getArrayCol()[i].nbImg<< "\n";
		}

		if (simData->getNbSatelite() == 0)
			std::cout << "[E] Error on reading file : No such file." << std::endl;

		std::cout << "[S] Success."  << std::endl;

		NaiveResolver * naive = new NaiveResolver(simData, outfilename);
		naive->launchResolution(true);

	
	

	

	system("pause");

	
}

