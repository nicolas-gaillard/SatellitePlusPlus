#include "DataReceiver.h"
#include "NaiveResolver.h"
#include <iostream>
#include <string>

int main(int argc, char *argv[]) {

	std::string filename(argv[1]);
	std::string outfilename;

	/*
	std::cout << argv[0] << std::endl;
	std::cout << argv[1] << std::endl;
	std::cout << typeid(argv[1]).name() << std::endl;

	std::cout << filename << std::endl;
	std::cout << typeid(filename).name()<< std::endl;
	// std::cout << argv[2] << std::endl;
	*/

	if (argc == 3) outfilename = argv[2];
	else outfilename = filename + "_out";

	if (argc > 3) return 0;

	// std::string filename = "text/weekend.in";

	std::cout << "================================" << std::endl;
	std::cout << "\tSATELLITE PLUS PLUS" << std::endl;
	std::cout << "================================" << std::endl;
	std::cout << "[I] Starting naive resolution..." << std::endl;

	DataReceiver * dataReceiver = new DataReceiver(filename, 15, 1);
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

	/*
	DataReceiver * dataReceiver = new DataReceiver(filename, 15);
	SimulationData d = dataReceiver->extractData();
	SimulationData * simData = &d;

	if (simData->getNbSatelite() == 0)
		std::cout << "[E] Error on reading file : No such file." << std::endl;

	std::cout << "[S] Success." << std::endl;

	NaiveResolver * naive = new NaiveResolver(simData, outfilename);
	naive->launchResolution(true);
	*/
	

	/*

	std::cout << "[I] Reading data from  '" << filename << "'... " << std::endl;
	for (size_t i = 20; i < 50; i=i+5)
	{
		//std::string outfilename = "text/" +std::to_string(i)+"weekend.out";
		DataReceiver * dataReceiver = new DataReceiver(filename,i);
		SimulationData d = dataReceiver->extractData();
		SimulationData * simData = &d;

		if (simData->getNbSatelite() == 0)
			std::cout << "[E] Error on reading file : No such file." << std::endl;

		std::cout << "[S] Success." << i << std::endl;

		NaiveResolver * naive = new NaiveResolver(simData, outfilename);
		naive->launchResolution(true);

	}
	

	*/

	// system("pause");

	
}

