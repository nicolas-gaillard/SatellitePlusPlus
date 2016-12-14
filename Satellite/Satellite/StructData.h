#pragma once
#include <string>


struct Image
{
	long la;
	long lo;
	int nbSat;
};

struct Satelite
{
	long la;
	long lo;
	int speed;
	int speedRot;
	int maxRot;
	Image * lastShotRelativePosition;
	int lastShotTurn;
	int nbImage;
};

struct TimeStamp {
	int minTime;
	int maxTime;
};

struct Collection
{
	int nbImg;
	int nbPts;
	int nbTimeSt;
	TimeStamp * listTimeSt;
	Image * listImg;
	bool doable;

};

