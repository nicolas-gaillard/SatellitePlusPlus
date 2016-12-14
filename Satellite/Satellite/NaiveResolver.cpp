#include "NaiveResolver.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <numeric>
#include <chrono>
#include <thread> 

NaiveResolver::NaiveResolver(SimulationData * simDat, std::string filename) {
	simData = simDat;
	outFilename = filename;
	available = std::vector<Collection*>();
	res = std::vector<Result>();

}

/*
Check if an image can be taken by a satelite without checking conflicts
*/
bool isInRange(Satelite * sat, Image * im, bool debug=false) {
	if (debug) {
		std::cout << std::endl << "Lat Sat : " << sat->la << std::endl;
		std::cout << "Lat im : " << im->la << std::endl;
		std::cout << "Sat - Im : " << std::abs(sat->la - im->la) << std::endl;
		std::cout << "Max Rotation : " << sat->maxRot << std::endl;
	}
	return (std::abs(sat->la - im->la) <= sat->maxRot) && (std::abs(sat->lo - im->lo) < sat->maxRot);
}

/*
Check if a picture can be taken at the given turn for the given collection.
*/
bool isInTimeStamp(int turn, Collection coll) {

	for (int i = 0; i < coll.nbTimeSt; i++)
	{
		TimeStamp ts = coll.listTimeSt[i];
		if (turn >= ts.minTime && turn <= ts.maxTime) {
			return true;
		}
	}

	return false;
}

/*
Check a possible conflict if the satelite take the given image at the given turn.
*/
bool isConflict(Satelite * sat, Image im, int turn) {
	// Get the position of the previous picture taken by the satelite, relative to it.
	Image * a = sat->lastShotRelativePosition;
	
	// Compute the relative position of the input image
	Image b;
	b.la = sat->la - im.la;
	b.lo = sat->lo - im.lo;
	
	// If there is no previous image taken, then the relative position is (0,0)
	if (a == NULL) {
		a = new Image;
		a->la = 0;
		a->lo = 0;
		// We return the fact that there is a conflict or not.
		return std::sqrt(std::pow(b.la - a->la, 2.0) + std::pow(b.lo - a->lo, 2.0)) > (sat->speedRot * (turn - sat->lastShotTurn));
	}
	else {
		// We return the fact that there is a conflict or not.
		return std::sqrt(std::pow(b.la - a->la, 2.0) + std::pow(b.lo - a->lo, 2.0)) > (sat->speedRot * (turn - sat->lastShotTurn));
	}

	
	
}

/*
Move the given satelite at his position one turn after.
*/
void moveSatelite(Satelite * sat) {
	// Initialize the new coordinates
	int new_lo = 0;
	int new_la = 0;
	int new_speed = 0;

	// Compute the new coordinates
	if (sat->la + sat->speed >= -90 * 60 * 60 && sat->la + sat->speed <= 90 * 60 * 60) {
		new_speed = sat->speed;
		new_la = sat->la + sat->speed; new_lo = sat->lo - 15;
	}
	else if (sat->la + sat->speed>90 * 60 * 60) {
		new_speed = -sat->speed;
		new_la = 180 * 60 * 60 - (sat->la + sat->speed); new_lo = -180 * 60 * 60 + sat->lo - 15;
	}
	else if (sat->la + sat->speed < -90 * 60 * 60) {
		new_speed = -sat->speed;
		new_la = -180 * 60 * 60 - (sat->la + sat->speed); new_lo = -180 * 60 * 60 + sat->lo - 15;
	}
	if (new_lo < -648000)new_lo += 360 * 60 * 60;
	if (new_lo > 647999)new_lo -= 360 * 60 * 60;

	// Set the new coordinates to the satelite
	sat->lo = new_lo;
	sat->la = new_la;
	sat->speed = new_speed;

}


void NaiveResolver::threadResolv(int i, int n, bool verbose, std::string * result) {
	int maxTurns = simData->getDuration();
	int satNb = simData->getNbSatelite();
	int memetps;

	// For each turn of the simulation...
	for (size_t x = i; x < satNb; x = x + n)
	{
		auto start = std::chrono::high_resolution_clock::now();
		Satelite * sat = &simData->getArraySat()[x];

		// For each satelite...
		for (int turn = 0; turn < maxTurns; turn++) {
			memetps = 0;
			// We get the current satelite, for better understanding
			// ... for each existing collection ...
			for (int j = 0; j < this->available.size(); j++) {
				// We get the current collection for better understanding
				Collection coll = *available[j];
				// Is the right time to take a picture in this collection ?
				if (isInTimeStamp(turn, coll)) {
					// Yes, so we can iterate through all its images.
					for (int k = 0; k < coll.nbImg; k++) {
						// If the image can be shot, and there is no conflict, then we take the picture.
						if (isInRange(sat, &coll.listImg[k]) && !isConflict(sat, coll.listImg[k], turn)) {

						}

					}
				}
			}
			if (memetps > 1)
				std::cout << " meme temps nombre " << memetps << std::endl;
			// And finally, we move the satelite.
			moveSatelite(sat);

		}

		if (verbose) {
			auto end = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double> diff = end - start;
			std::cout << " satelite  " << x << " thread no " << i << "/" << n << " time elapsed " << diff.count() << " s\n";
		}

	}

}

void NaiveResolver::threadPrepResolv(int i, int n ,bool verbose,std::string * result) {
	int maxTurns = simData->getDuration();
	int satNb = simData->getNbSatelite();
	int colNb = simData->getNbCollection();
	int memetps;
	// For each turn of the simulation...
	for (size_t x = i; x < satNb; x = x + n)
	{
		auto start = std::chrono::high_resolution_clock::now();
		Satelite * sat = &simData->getArraySat()[x];

		// For each satelite...
		for (int turn = 0; turn < maxTurns; turn++) {
			memetps = 0;
			// We get the current satelite, for better understanding
			// ... for each existing collection ...
			for (int j = 0; j < colNb; j++) {
				// We get the current collection for better understanding
				Collection coll = simData->getArrayCol()[j];
				// Is the right time to take a picture in this collection ?
				if (isInTimeStamp(turn, coll)) {
					// Yes, so we can iterate through all its images.
					for (int k = 0; k < coll.nbImg; k++) {
						// If the image can be shot, and there is no conflict, then we take the picture.
						if (isInRange(sat, &coll.listImg[k]) ) {
							
						}
						
					}
				}
			}
			if (memetps > 1)
				std::cout << " meme temps nombre " << memetps << std::endl;
			// And finally, we move the satelite.
			moveSatelite(sat);

		}

		if (verbose) {
			auto end = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double> diff = end - start;
			std::cout << " satelite  " << x << " thread no " << i << "/" << n << " time elapsed " << diff.count() << " s\n";
		}

	}

}
void NaiveResolver::checkDoable() {
	int cptzero = 0;
	int cptmoyenne = 0;
	int cptimage = 0;
	int cptcoll = 0;

	for (size_t i = 0; i < simData->getNbCollection(); i++)
	{
		Collection c = simData->getArrayCol()[i];
		for (size_t j = 0; j < c.nbImg; j++)
		{
			if (c.listImg[j].nbSat == 0) {
				cptzero += c.nbImg;
				c.doable = false;
				break;
			}
			cptmoyenne += c.listImg[j].nbSat;

		}

		if (c.doable) {
			cptcoll++;
			this->available.push_back(&simData->getArrayCol()[i]);
			
		}

		cptimage += c.nbImg;
	}

	for (size_t i = 0; i < available.size(); i++)
	{
		if (available[i]->doable == false)
			std::cout << "trololololololo";
	}

	std::cout << " nb image with 0 satelite " << cptzero << " nb average satperimage " << cptmoyenne / cptimage << std::endl;
	std::cout << "nb coll doable " << cptcoll << " out of " << simData->getNbCollection() << std::endl;
}



void NaiveResolver::launchResolution(bool verbose) {
	// Get data for better reading.
	int satNb = simData->getNbSatelite();
	int tmp = 4;
	std::thread *t = new std::thread[tmp];
	NaiveResolver * th = this;
	this->nbConflict = 0;

	// Initialize the variables which stores the resulting data
	std::string * result = new std::string[tmp] ;



	if (verbose) {
		std::cout << "[I] Start simulation ..." << std::endl;
		std::cout << "\r[I] Complete : 0%";
	}

	for (int i = 0; i < tmp; i++) {
		result[i] = std::string();
		t[i] = std::thread([&th, i, tmp,result,verbose]() { th->threadPrepResolv(i, tmp,verbose,result); });
	}
	for (size_t i = 0; i < tmp; i++)
	{
		t[i].join();
	}

	std::cout << "nb conf" << nbConflict << std::endl;
	this->checkDoable();
	// When the simulation is over, we write in the output file.
	/*std::ofstream myfile;
	myfile.open(outFilename);
	myfile << std::to_string(nbPict) << std::endl;
	for (size_t i = 0; i < tmp; i++)
		myfile << result[i];

	myfile.close();*/
	

}