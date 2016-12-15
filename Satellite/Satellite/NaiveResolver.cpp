#include "NaiveResolver.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <numeric>
#include <chrono>
#include <thread> 
#include <iterator>

NaiveResolver::NaiveResolver(SimulationData * simDat, std::string filename) {
	notdoable = std::set<int>();
	currentlyProcessing = true;
	simData = simDat;
	outFilename = filename;
	available = std::vector<Collection*>();
	res = std::vector<Result>();
	initialData = new Satelite[simData->getNbSatelite()];
	initialCollection = new Collection[simDat->getNbCollection()];
	memcpy(initialData, simData->getArraySat(), sizeof(Satelite)*simData->getNbSatelite());
	memcpy(initialCollection, simData->getArrayCol(), sizeof(Satelite)*simData->getNbCollection());

}

void NaiveResolver::resetSat() {
	memcpy(simData->getArraySat(), initialData, sizeof(Satelite)*simData->getNbSatelite());
	memcpy(simData->getArrayCol(), initialCollection, sizeof(Satelite)*simData->getNbCollection());
	
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


bool hadTakenPhoto(Collection * coll) {
	for (size_t i = 0; i < coll->nbImg; i++)
	{
		if (coll->listImg[i].taken)
			return true;
	}
	return false;
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

int NaiveResolver::makeChoice(std::vector<Image*>possible, Satelite *sat ) {
	int valmax = 0;
	int idxvalmax ;
	int val;
	Image * img;
	Collection c;
	for (size_t i = 0; i < possible.size(); i++)
	{
		img = possible[i];
		c = simData->getArrayCol()[img->coll];
		val = c.nbPts / (img->nbSat * c.nbPtsLeft * c.nbPtsLeft);
		if (val > valmax)
			idxvalmax = i;
	}
	return idxvalmax;
}
/*
void NaiveResolver::afterChoice(std::vector<Image*> possible, int idxChoice,Satelite *sat,int turn){
	Result res;
	Image tmp_im;
	for (size_t idx = 0; idx < possible.size(); idx++)
	{
		std::cout << "Image la " << possible[idx]->la << " lo " << possible[idx]->lo << " coll nul " << possible[idx]->coll << std::endl;
		if (idx == idxChoice) {
			res.img = possible[idx];
			continue;
		}
		possible[idx]->nbSat--;
		if (possible[idx]->nbSat == 0)
			this->simData->getArrayCol()[possible[idx]->coll].doable = false;
	}
	std::cout << this->simData->getArrayCol()[res.img->coll].nbPtsLeft << std::endl;
	this->simData->getArrayCol()[res.img->coll].nbPtsLeft--;
	res.la = sat->la;
	res.lo = sat->lo;
	res.turn = turn;
	res.img->taken = true;

	tmp_im.la = sat->la - res.img->la;
	tmp_im.lo = sat->lo - res.img->lo;
	sat->lastShotRelativePosition = &tmp_im;
	sat->lastShotTurn = turn;
	// And we save the result in our result string.
	
	this->res.push_back(res);
}
*/

void NaiveResolver::threadResolv(int i, int n, bool verbose) {
	int maxTurns = simData->getDuration();
	const int satNb = simData->getNbSatelite();
	int idxChoice;
	Satelite tmp;
	Result r;
	
	std::vector<Image*> possible = std::vector<Image*>();
	// For each turn of the simulation...
	for (size_t x = i; x < satNb; x = x + n)
	{
		auto start = std::chrono::high_resolution_clock::now();
		Satelite * sat = &simData->getArraySat()[x];

		// For each satelite...
		for (int turn = 0; turn < maxTurns; turn++) {
			// We get the current satelite, for better understanding
			// ... for each existing collection ...
			for (int j = 0; j < this->simData->getNbCollection(); j++) {
				// We get the current collection for better understanding
				Collection coll = simData->getArrayCol()[j];
				if (notdoable.find(j) == notdoable.end() && isInTimeStamp(turn, coll)) {
					// Is the right time to take a picture in this collection ?
						// Yes, so we can iterate through all its images.
						for (int k = 0; k < coll.nbImg; k++) {
							if (!this->currentlyProcessing) {
								if(verbose)
									std::cout << "Choice step satelite  " << x << " thread no " << i << "/" << n << " get resetted s\n";
								return;
							}
							// If the image can be shot, and there is no conflict, then we take the picture.
							if (isInRange(sat, &coll.listImg[k])  && !coll.listImg[k].taken) {
								//possible.push_back(&coll.listImg[k]);
								if (!isConflict(sat, coll.listImg[k], turn)) {
									coll.listImg[k].taken = true;
									coll.nbPtsLeft--;
									Image tmp_im = coll.listImg[k];
									tmp_im.la = sat->la - coll.listImg[k].la;
									tmp_im.lo = sat->lo - coll.listImg[k].lo;
									sat->lastShotRelativePosition = &tmp_im;
									sat->lastShotTurn = turn;
									r.sat = x;
									r.la = coll.listImg[k].la;
									r.lo = coll.listImg[k].lo;
									r.turn = turn;
									res.push_back(r);
								}
								else if(coll.listImg[k].nbSat-1 == 0 && hadTakenPhoto(&coll)) {
									this->currentlyProcessing = false;
									wrongcoll = j;
									if(verbose)
										std::cout << "Choice step satelite  " << x << " thread no " << i << "/" << n << "  reseting simul s\n";

									return;
								}
								else
									coll.listImg[k].nbSat--;
							}

						}
					

				}
				
			}
			/*
			if (possible.size() != 0) {
				idxChoice = this->makeChoice(possible, sat);
				this->afterChoice(possible, idxChoice, sat, turn);
				possible.clear();
			}*/
		 	
			// And finally, we move the satelite.
			moveSatelite(sat);

		}

		if (verbose) {
			auto end = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double> diff = end - start;
			std::cout << "Choice step satelite  " << x << " thread no " << i << "/" << n << " time elapsed " << diff.count() << " s\n";
		}

	}
	std::cout << "nb photos prises " << this->res.size() << std::endl;

}

void NaiveResolver::threadPrepResolv(int i, int n ,bool verbose,std::string * result) {
	int maxTurns = simData->getDuration();
	int satNb = simData->getNbSatelite();
	int colNb = simData->getNbCollection();
	// For each turn of the simulation...
	for (size_t x = i; x < satNb; x = x + n)
	{
		auto start = std::chrono::high_resolution_clock::now();
		Satelite * sat = &simData->getArraySat()[x];

		// For each satelite...
		for (int turn = 0; turn < maxTurns; turn++) {
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
							coll.listImg[k].nbSat++;
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
				cptzero++;
				c.doable = false;
				break;
			}
			cptmoyenne += c.listImg[j].nbSat;

		}
		if (c.doable) 
			cptcoll++;			
		
		cptimage += c.nbImg;
	}

	

	std::cout << " nb image with 0 satelite " << cptzero << " nb average satperimage " << cptmoyenne / cptimage << std::endl;
	std::cout << "nb coll doable " << cptcoll << " out of " << simData->getNbCollection() << std::endl;
}


void NaiveResolver::threadResolv(int i, int n, bool verbose, std::string * result) {
	int maxTurns = simData->getDuration();
	int satNb = simData->getNbSatelite();
	int colNb = simData->getNbCollection();
	// For each turn of the simulation...
	for (size_t x = i; x < satNb; x = x + n)
	{
		auto start = std::chrono::high_resolution_clock::now();
		Satelite * sat = &simData->getArraySat()[x];
		// For each satelite...
		for (int turn = 0; turn < maxTurns; turn++) {
			// We get the current satelite, for better understanding
			// ... for each existing collection ...
			for (int j = 0; j < colNb; j++) {

				// We get the current collection for better understanding
				

				if (notdoable.find(j) == notdoable.end()) {
					Collection coll = simData->getArrayCol()[j];
					// Is the right time to take a picture in this collection ?
					if (isInTimeStamp(turn, coll)) {
						// Yes, so we can iterate through all its images.
						for (int k = 0; k < coll.nbImg; k++) {
							// If the image can be shot, and there is no conflict, then we take the picture.
							//std::cout << "num " << sat->la << " out of " << coll.nbImg << std::endl;
							//std::cout << "num " << coll.listImg[k].la << " out of " << coll.nbImg << std::endl;

							if (isInRange(sat, &coll.listImg[k]) && !isConflict(sat, coll.listImg[k], turn) && !coll.listImg[k].taken) {
								// We set the last shot of the satelite to be this picture, at this turn.
								coll.listImg[k].taken = true;
								Image tmp_im = coll.listImg[k];
								tmp_im.la = sat->la - coll.listImg[k].la;
								tmp_im.lo = sat->lo - coll.listImg[k].lo;
								sat->lastShotRelativePosition = &tmp_im;
								sat->lastShotTurn = turn;
								// And we save the result in our result string.
								result[i] += std::to_string(coll.listImg[k].la) + " ";
								result[i] += std::to_string(coll.listImg[k].lo) + " ";
								result[i] += "" + std::to_string(turn);
								result[i] += " " + std::to_string(x);
								result[i] += '\n';
								// we don't forget to increment the number of pictures taken, isn't it ?
								nbPict++;

							}
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
			std::cout << "[I] Number of picture taken : " << nbPict << std::endl;
		}
	}
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



	if (verbose) 
		std::cout << "[I] Start simulation ..." << std::endl;
	

	for (int i = 0; i < tmp; i++) {
		result[i] = std::string();
		t[i] = std::thread([&th, i, tmp,result,verbose]() { th->threadPrepResolv(i, tmp,verbose,result); });
	}
	for (size_t i = 0; i < tmp; i++)
	{
		t[i].join();
	}
	verbose = false;
	this->checkDoable();
	int doable = 0;
	for (size_t c = 0; c < simData->getNbCollection(); c++)
		if (simData->getArrayCol()[c].doable)
		doable++;
	std::cout << " nombre doable " << doable << std::endl;
	do {
		res.clear();
		this->resetSat();
		currentlyProcessing = true;
		for (int i = 0; i < tmp; i++)
			t[i] = std::thread([&th, i, tmp, result, verbose]() { th->threadResolv(i, tmp, verbose); });

		for (size_t i = 0; i < tmp; i++)
			t[i].join();

		if (currentlyProcessing || notdoable.size()>=200)
			break;
		notdoable.insert(wrongcoll);

		for (size_t c = 0; c < simData->getNbCollection(); c++) {
			if (notdoable.find(c) == notdoable.end()) {
				for (size_t img = 0; img < simData->getArrayCol()[c].nbImg; img++) {
					simData->getArrayCol()[c].listImg[img].taken = false;
					simData->getArrayCol()[c].nbPtsLeft = simData->getArrayCol()[c].nbImg;
				}
			}
			
		}
		
		std::cout << " num doable " << wrongcoll << "nondoable" << notdoable.size() << std::endl;

	} while (!currentlyProcessing);
	std::string filename = "text/constellation.in";
	DataReceiver * dataReceiver = new DataReceiver(filename);
	SimulationData d = dataReceiver->extractData();
	simData = &d;
	for (int i = 0; i < tmp; i++) {
		result[i] = std::string();
		t[i] = std::thread([&th, i, tmp, result, verbose]() { th->threadResolv(i, tmp, true, result); });
	}
	for (size_t i = 0; i < tmp; i++)
	{
		t[i].join();
	}
	
	
	std::ofstream myfile;
	myfile.open(outFilename);
	myfile << std::to_string(nbPict) << std::endl;
	for (size_t i = 0; i < tmp; i++)
		myfile << result[i];
	myfile.close();

}
