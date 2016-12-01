#pragma once
#include <string>



struct Satelite
{
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

	// Test :
	/*
	bool operator< (const Image& img) const
    {
        return (this->la < img.la && this->lo < img.lo);
    }
    */

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

