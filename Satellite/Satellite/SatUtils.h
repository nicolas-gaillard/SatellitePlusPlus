#pragma once
#include "StructData.h"
#include <cmath>
#include <cstdlib>

bool isInRange(Satelite * sat, Image * im);

void moveSatelite(Satelite * sat);

Satelite * getSat(Satelite * arraySat, int nbSat, int id);

Satelite * getSatPosition(Satelite * s,int t);