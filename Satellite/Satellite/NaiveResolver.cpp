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
	Image * a = sat->lastShotRelativePosition;

	if (a != NULL) {
		Image b;
		b.la = sat->la - im.la;
		b.lo = sat->lo - im.lo;

		return std::sqrt(std::pow(b.la - a->la, 2.0) + std::pow(b.lo - a->lo, 2.0)) > (sat->speedRot * (turn - sat->lastShotTurn));
	}
	else {
		return false;
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
	if (sat->la + sat->speed >= -324000 && sat->la + sat->speed <= 324000) {
		new_speed = sat->speed;
		new_la = sat->la + sat->speed; new_lo = sat->lo - 15;
	}
	else if (sat->la + sat->speed>9324000) {
		new_speed = -sat->speed;
		new_la = 648000 - (sat->la + sat->speed); new_lo = -648000 + sat->lo - 15;
	}
	else if (sat->la + sat->speed < -324000) {
		new_speed = -sat->speed;
		new_la = -648000 - (sat->la + sat->speed); new_lo = -648000 + sat->lo - 15;
	}
	if (new_lo < -648000)new_lo += 1296000;
	if (new_lo > 647999)new_lo -= 1296000;

	// Set the new coordinates to the satelite
	sat->lo = new_lo;
	sat->la = new_la;
	sat->speed = new_speed;

}


void NaiveResolver::threadResolv(int i, int n ,bool verbose,std::string * result) {
	int maxTurns = simData->getDuration();
	int satNb = simData->getNbSatelite();
	int colNb = simData->getNbCollection();

	// For each turn of the simulation...
	for (size_t x = i; x < satNb; x = x + n)
	{
		auto start = std::chrono::high_resolution_clock::now();

		// For each satelite...
		for (int turn = 0; turn < maxTurns; turn++) {
			// We get the current satelite, for better understanding
			Satelite * sat = &simData->getArraySat()[i];
			// ... for each existing collection ...
			for (int j = 0; j < colNb; j++) {
				// We get the current collection for better understanding
				Collection coll = simData->getArrayCol()[j];
				// Is the right time to take a picture in this collection ?
				if (isInTimeStamp(turn, coll)) {
					// Yes, so we can iterate through all its images.
					for (int k = 0; k < coll.nbImg; k++) {
						// If the image can be shot, and there is no conflict, then we take the picture.
						if (isInRange(sat, &coll.listImg[k]) && !isConflict(sat, coll.listImg[k], turn)) {

							// We set the last shot of the satelite to be this picture, at this turn.
							Image tmp_im = coll.listImg[k];
							tmp_im.la = sat->la - coll.listImg[k].la;
							tmp_im.lo = sat->lo - coll.listImg[k].lo;
							sat->lastShotRelativePosition = &tmp_im;
							sat->lastShotTurn = turn;

							// And we save the result in our result string.
							result[i] += std::to_string(coll.listImg[k].la) + " ";
							result[i] += std::to_string(coll.listImg[k].lo) + " ";
							result[i] += "" + std::to_string(turn);
							result[i] += " " + std::to_string(i);
							result[i] += '\n';
							// we don't forget to increment the number of pictures taken, isn't it ?
							nbPict++;
						}
					}
				}
			}
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

void NaiveResolver::launchResolution(bool verbose) {
	// Get data for better reading.
	int satNb = simData->getNbSatelite();
	int tmp = 4;
	std::thread *t = new std::thread[tmp];
	NaiveResolver * th = this;

	// Initialize the variables which stores the resulting data
	std::string * result = new std::string[tmp] ;

	if (verbose) {
		std::cout << "[I] Start simulation ..." << std::endl;
		std::cout << "\r[I] Complete : 0%";
	}

	for (int i = 0; i < tmp; i++) {
		result[i] = std::string();
		t[i] = std::thread([&th, i, tmp,result,verbose]() { th->threadResolv(i, tmp,verbose,result); });
	}
	for (size_t i = 0; i < tmp; i++)
	{
		t[i].join();
	}


	
	// When the simulation is over, we write in the output file.
	std::ofstream myfile;
	myfile.open(outFilename);
	myfile << std::to_string(nbPict) << std::endl;
	for (size_t i = 0; i < tmp; i++)
		myfile << result[i];

	myfile.close();
	

}