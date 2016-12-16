#pragma once
#include <string>


struct Image
{
	long la;
	long lo;
	int nbSat;
	bool taken;
	int  coll;
};

struct Satelite
{
	long la;
	long lo;
	int speed;
	int speedRot;
	int maxRot;
	Image *lastShotRelativePosition;
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
	int nbPtsLeft;
	bool doable;

};


struct Result
{
	Image * img;
	int sat;
	int la;
	int lo;
	int turn;
};
