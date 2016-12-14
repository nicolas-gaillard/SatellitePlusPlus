#include "SatUtils.h"

/*
 * Check if an image can be taken by a satelite
*/
bool isInRange(Satelite * sat, Image * im) {
	return (std::abs(sat->la - im->la) <= sat->maxRot) && (std::abs(sat->lo - im->lo) < sat->maxRot);
}


/*
* Move the given satelite at his position one turn after.
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

/*
 * Get the satelite from arraySat with his id
 * return the satelite
*/
Satelite * getSat(Satelite * arraySat, int nbSat, int id) {
    Satelite * sat;
    for (int i = 0; i < nbSat; i++) {
        sat = &arraySat[i];
        if (sat->id == id) {
            return sat;
        }
    }
    return nullptr;
}

/*
 * Give the position of a satelite at turn t 
 * Satelite s have to be in initial position
 * Return a new satelite with a new position 
*/
Satelite * getSatPosition(Satelite * s,int t) {
    Satelite * sat = new Satelite();

    sat->la = s->la;
    sat->lo = s->lo;
    sat->maxRot = s->maxRot;
    sat->speed = s->speed;
    sat->speedRot = s->speedRot;

    for (int i = 0; i < t; i++) {
        moveSatelite(sat);
    }
    return sat;
}