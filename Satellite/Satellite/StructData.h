#pragma once
#include <string>



struct Satelite
{
	int id;
	long la;
	long lo;
	int speed;
	int speedRot;
	int maxRot;
};


struct Image
{
	long la;
	long lo;

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

