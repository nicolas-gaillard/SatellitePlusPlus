#include "DataReceiver.h"


void split(const std::string &s, char delim, std::vector<std::string> &elems) {
	std::stringstream ss;
	ss.str(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
}

DataReceiver::DataReceiver(std::string filename)
{
	infile =  new std::ifstream(filename);
}

SimulationData DataReceiver::extractData() {

	std::string line ;
	std::vector<std::string> elems;
	SimulationData  tmpData  ;


	// duration of the simulation
	getline(*infile, line);
	tmpData.setDuration(atoi(line.c_str()));
	tmpData.setArraySat(this->extractSatelite(&tmpData));
	tmpData.setArrayCol(this->optiExtractCollection(&tmpData, 20));

	
	return tmpData;

}

Satelite *  DataReceiver::extractSatelite(SimulationData * Sd) {

	std::string line;
	std::vector<std::string> elems;

	// total of satelites lines
	getline(*infile, line);
	const int nb = atoi(line.c_str());
	Sd->setNbSatelite(nb);
	Satelite * arraySatelite = new Satelite[nb];
	Satelite * tmp;


	for (size_t i = 0; i < nb; i++)
	{
		tmp = new Satelite();
		getline(*infile, line);
		split(line, ' ', elems);
		tmp->la = atoi(elems[0].c_str());
		tmp->lo = atoi(elems[1].c_str());
		tmp->speed = atoi(elems[2].c_str());
		tmp->speedRot = atoi(elems[3].c_str());
		tmp->maxRot = atoi(elems[4].c_str());
		arraySatelite[i] = *tmp;
		elems.clear();
	}
	

	Sd->setArraySat(arraySatelite);

	return arraySatelite;

}


Collection *  DataReceiver::extractCollection(SimulationData * Sd) {

	std::string line;
	std::vector<std::string> elems;

	// total of Collections 
	getline(*infile, line);
	const int nb = atoi(line.c_str());
	Sd->setNbCollection(nb);
	Collection * arrayCollection = new Collection[nb];
	Collection * tmp;


	for (size_t i = 0; i < nb; i++)
	{
		tmp = new Collection();
		getline(*infile, line);
		split(line, ' ', elems);

		tmp->nbPts = atoi(elems[0].c_str());
		tmp->nbImg = atoi(elems[1].c_str());
		tmp->nbTimeSt = atoi(elems[2].c_str());

		tmp->listImg = this->extractImage(tmp->nbImg);
		tmp->listTimeSt = this->extractTimeStamp(tmp->nbTimeSt);
		//std::cout << "ok";

		tmp->isValid = true;

		arrayCollection[i] = *tmp;
		elems.clear();
	}


	return arrayCollection;

}


Image *  DataReceiver::extractImage(int nb) {

	std::string line;
	std::vector<std::string> elems;
	// total of Images per collection
	Image * arrayImage = new Image[nb];
	Image * tmp;


	for (size_t i = 0; i < nb; i++)
	{
		tmp = new Image();
		getline(*infile, line);
		split(line, ' ', elems);
		//std::cout << "taille" << elems.size() << std::endl;
		//std::cout << line << std::endl;

		tmp->la = atoi(elems[0].c_str());
		tmp->lo = atoi(elems[1].c_str());
		tmp->taken = false;
		arrayImage[i] = *tmp;
		elems.clear();
	}
	return arrayImage;

}



TimeStamp *  DataReceiver::extractTimeStamp(int nb) {

	std::string line;
	std::vector<std::string> elems;
	// total of timestamp
	TimeStamp * arrayTimeStamp = new TimeStamp[nb];
	TimeStamp * tmp;


	for (size_t i = 0; i < nb; i++)
	{
		tmp = new TimeStamp();
		getline(*infile, line);
		//std::cout << line << std::endl;

		split(line, ' ', elems);
		tmp->minTime = atoi(elems[0].c_str());
		tmp->maxTime = atoi(elems[1].c_str());
		arrayTimeStamp[i] = *tmp;
		elems.clear();
	}
	return arrayTimeStamp;

}


Collection * DataReceiver::optiExtractCollection(SimulationData * Sd, int threshold){
	Collection * oldArrayCol = this->extractCollection(Sd);
	
	std::vector<Collection> vecCollection;

	std::cout << Sd->getNbCollection() << std::endl;

	for (int i = 0; i < Sd->getNbCollection(); i++)
	{
		vecCollection.push_back(oldArrayCol[i]);
	}

	std::sort(vecCollection.begin(), vecCollection.end(), 
			[](const Collection &a, const Collection &b) -> bool
				{ 
					return ((a.nbPts/a.nbImg) > (b.nbPts/b.nbImg));
				}
			);

	int newSize = static_cast<int>(static_cast<float>(vecCollection.size()) * (1.0 - static_cast<float>(threshold)/100.0));

	Collection * arrayCollection = new Collection[newSize];

	for (int j = 0; j < newSize; j++)
	{
		arrayCollection[j] = vecCollection[j]; 
	}

	return arrayCollection;
}