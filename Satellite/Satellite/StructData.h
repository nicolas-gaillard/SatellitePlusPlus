#pragma once
#include <string>


struct Image
{
	long la;
	long lo;
	bool taken;
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

};

