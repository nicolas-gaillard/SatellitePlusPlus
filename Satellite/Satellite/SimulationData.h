#pragma once

#include "StructData.h"

class SimulationData
{

public:
	SimulationData();
	~SimulationData();


	inline long getNbCollection() { return nbCollection; };
	inline long getDuration() { return duration; };
	inline long getNbSatelite() { return nbSatelite; };
	inline Satelite * getArraySat() { return arraySat; };
	inline Collection * getArrayCol() { return arrayCol; };

	inline void setNbCollection(int nb) { this->nbCollection = nb; };
	inline void setDuration(long d) { this->duration = d; };
	inline void setNbSatelite(int nb) { this->nbSatelite = nb; };
	inline void setArraySat(Satelite * s) { this->arraySat = s; };
	inline void setArrayCol(Collection * c) { this->arrayCol = c; };

private :
	long duration;
	int nbSatelite;
	int nbCollection;
	Satelite * arraySat;
	Collection * arrayCol;
};

