#include "NaiveResolver.h"
#include <iostream>
#include <fstream>
#include <cmath>

NaiveResolver::NaiveResolver(SimulationData * simDat, std::string filename) {
	simData = simDat;
	outFilename = filename;
}

bool isInRange(Satelite * sat, Image * im, bool debug=false) {
	if (debug) {
		std::cout << std::endl << "Lat Sat : " << sat->la << std::endl;
		std::cout << "Lat im : " << im->la << std::endl;
		std::cout << "Sat - Im : " << std::abs(sat->la - im->la) << std::endl;
		std::cout << "Max Rotation : " << sat->maxRot << std::endl;
	}
	return (std::abs(sat->la - im->la) <= sat->maxRot) && (std::abs(sat->lo - im->lo) < sat->maxRot);
}

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


void moveSatelite(Satelite * sat) {
	int new_lo = 0;
	int new_la = 0;
	int new_speed = 0;

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

	sat->lo = new_lo;
	sat->la = new_la;
	sat->speed = new_speed;

}

void NaiveResolver::launchResolution() {
	int maxTurns = simData->getDuration();
	int satNb = simData->getNbSatelite();
	int colNb = simData->getNbCollection();

	std::string result = "";
	int nbPict = 0;

	std::cout << "[I] Start simulation ..." << std::endl;
	std::cout << "\r[I] Complete : 0%";

	for (int turn = 0; turn < maxTurns; turn++)
	{
		for (int i = 0; i < satNb; i++) {
			Satelite * sat = &simData->getArraySat()[i];
			for (int j = 0; j < colNb; j++)	{
				Collection coll = simData->getArrayCol()[j];

				if (isInTimeStamp(turn, coll)) {
					for (int k = 0; k < coll.nbImg; k++) {
						if (isInRange(sat, &coll.listImg[k]) && !isConflict(sat, coll.listImg[k], turn)) {
							Image imtmp = coll.listImg[k];

							Image * tmp_im = &coll.listImg[k];
							tmp_im->la = sat->la - coll.listImg[k].la;
							tmp_im->lo = sat->lo - coll.listImg[k].lo;
							sat->lastShotRelativePosition = tmp_im;
							sat->lastShotTurn = turn;

							result += std::to_string(imtmp.la) + " ";
							result += std::to_string(imtmp.lo) + " ";
							result += "" + std::to_string(turn);
							result += " " + std::to_string(i);
							//std::cout << " - Satelite : " + std::to_string(sat->la) + " " + std::to_string(sat->lo) << " - Picture : " + std::to_string(imtmp.la) + " " + std::to_string(imtmp.lo) << std::endl;
							result += '\n';

							nbPict++;
						}
					}
				}
			}

			moveSatelite(sat);

		}


		std::cout << "\r[I] Complete : " << (turn*100)/maxTurns << "%";
	}

	std::cout << "\r[I] Complete.\t\t\t\t\t" << std::endl;
	
	std::ofstream myfile;
	myfile.open(outFilename);
	myfile << std::to_string(nbPict) << std::endl;
	myfile << result;
	myfile.close();

}