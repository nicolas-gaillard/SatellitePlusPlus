#include "DataReceiver.h"
#include "NaiveResolver.h"
#include <iostream>
#include <string>

int main() {



	std::string filename = "text/weekend.in";

	std::cout << "================================" << std::endl;
	std::cout << "\tSATELLITE PLUS PLUS" << std::endl;
	std::cout << "================================" << std::endl;
	std::cout << "[I] Starting naive resolution..." << std::endl;

	std::cout << "[I] Reading data from  '" << filename << "'... " << std::endl;
	for (size_t i = 20; i < 50; i=i+5)
	{
		std::string outfilename = "text/" +std::to_string(i)+"weekend.out";
		DataReceiver * dataReceiver = new DataReceiver(filename,i);
		SimulationData d = dataReceiver->extractData();
		SimulationData * simData = &d;

		if (simData->getNbSatelite() == 0)
			std::cout << "[E] Error on reading file : No such file." << std::endl;

		std::cout << "[S] Success." << i << std::endl;

		NaiveResolver * naive = new NaiveResolver(simData, outfilename);
		naive->launchResolution(true);

	}
	

	

	system("pause");

	
}

