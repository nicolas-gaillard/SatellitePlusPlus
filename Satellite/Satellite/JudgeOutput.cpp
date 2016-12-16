#include "JudgeOutput.h"
#include <iostream>
#include <cstdio>
#include <regex>

/*
 * Contructor 
 * filename : path of the output file
*/
JudgeOutput::JudgeOutput(std::string outFileName, SimulationData * s) {
    outputFile = new std::ifstream(outFileName);
    this->simData = s;
}

/*
 * Get back to the beginning of the file
*/ 
void JudgeOutput::goBackToBeginningOutput() {
    outputFile->clear();
    outputFile->seekg(0, std::ios::beg);
}

/*
 * Check if the output file is valid :
    * The format of the output file matches the description.
    * Each picture is in range of the corresponding satellite when it’s taken.
    * No satellite moves the camera faster than w arcseconds per turn between taking two consecutive pictures.
 * Return true if valid, false otherwise
*/
bool JudgeOutput::isValidOutput() {  
    return ((isValidSyntax())&&(isValidImages())&&(isValidCamera()));
}

/* 
 * Check if the file format syntax is valid. Check the semantic too
 * maxPictures: the number of pictures of the input file
 * maxTurns: number of the turn of the simulation
 * satNb: number of satelite in the simulation
 * Return true if valid, false otherwise
*/
bool JudgeOutput::isValidSyntax() {
    long maxTurns = simData->getDuration();
    int satNb = simData->getNbSatelite();
    int maxPictures = simData->getNbCollection();


	// ----- SYNTAX CHECK -----

	// Definition of usefull regular expressions.
	std::regex number_regex("[[:digit:]]+");
	std::regex line_regex("(-?[[:digit:]]+[[:space:]]){2}([[:digit:]]+[[:space:]][[:digit:]]+)");


	// Get of the first line
	std::string line;
	std::getline(*outputFile, line);

	// Test of the first line
	if (!std::regex_match(line, number_regex)) {
		// Test failed.
		std::cout << "[E] Test failed at line : '" << line << "'." << std::endl;
		return false;
	}

	// Convert number after check pass
	int picturesNb = std::stoi(line);

	// Creation of the picture array
	std::string * pictureArray = new std::string[picturesNb];

	// Test of each line
	int iter = 0;
	while (std::getline(*outputFile, line)) {
		if (!std::regex_match(line, line_regex)) {
			// Test failed.
			std::cout << "[E] Test failed at line " << iter << " :"  << "'" <<line << "'." << std::endl;
			return false;
		}
		else {
			// Test success : line saved.
			pictureArray[iter] = line;
			iter++;
		}
	}

	// ------- SEMANTICAL CHECK --------

	// Test of the number of pictures
	if (picturesNb > maxPictures || picturesNb <= 0) {
		return false;
	}

	std::vector<std::string> elems;
	int la, lo, tu, sa;

	// Test of each line
	for (int i = 0; i < picturesNb; i++)
	{  
		splitStr(pictureArray[i], ' ', elems);
		try
		{
			la = std::stoi(elems[0]);
			lo = std::stoi(elems[1]);
			tu = std::stoi(elems[2]);
			sa = std::stoi(elems[3]);
		}
		catch (const std::exception&)
		{
			return false;
		}
		if ((la < -306000) || (la > 306000) || (lo < -648000) || (lo > 647999) || (tu > maxTurns) || (tu < 0) || (sa+1 > satNb) || (sa < 0)) {
			return false;
		}
        elems.clear();
	}
    this->goBackToBeginningOutput();
	return true;
}

/*
 * Check all the images of the output
 * Return true if all images are valid, false otherwise
*/
bool JudgeOutput::isValidImages() {    
    Image * img;
    Satelite * sat = new Satelite();
    int turn;
    std::string line;
    bool b = true;

    // Get the array satelite and number of it
    Satelite * arraySat = this->simData->getArraySat();
    long nbSatelite = this->simData->getNbSatelite();

     // Get the first line : number of image taken
	std::getline(*outputFile, line);
    std::vector<std::string> elems;
	int nbImageTaken = std::stoi(line);

    // To compare in the for (first id is 0)
    sat->id = -1;
    
    
    // Browse all the images and check if it could have been taken by the satelite
    for (int i = 0; i < nbImageTaken; i++) {
        img = new Image();
        std::getline(*outputFile, line);

        splitStr(line, ' ', elems);

        img->la = std::stoi(elems[0]);
        img->lo = std::stoi(elems[1]);

        turn = std::stoi(elems[2]);

        // Change the satelite only if it different
        if (sat->id != std::stoi(elems[3])) {
            // Get the satelite from arraySat with his ID
            sat = getSat(arraySat, nbSatelite, std::stoi(elems[3]));
        }
       // std::cout << sat->la << " " << sat->lo << std::endl;
        //std::cout << "Test at turn " << turn << " with image " << img->la << " " << img->lo <<  " with sat ";
         // Test the current image
        if (!isValidImage(turn, img, sat)) {
            //std::cout << "[E] image not valid" << std::endl;
            b = false;
            break;
        }
       // std::cout << "[V] image ok" << std::endl;
        elems.clear();
    }
    this->goBackToBeginningOutput();
    return b;
}

/*
 * Check if the image img was in the range of the satelite sat at turn t
 * Return true if it was in range, false otherwise
*/
bool JudgeOutput::isValidImage(int t, Image * img, Satelite * sat) {
    return isInRange(getSatPosition(sat, t), img);
}

/*
 * Get all the images taken from the outputFile
 * Return a vector of all the images
*/
std::vector<Image> JudgeOutput::getImagesTaken() {
    std::vector<Image> imgs;
    Image * img;
    std::vector<std::string> elems;
	std::string line;

    // Get the first line : number of image taken
	std::getline(*outputFile, line);
	int nbImageTaken = std::stoi(line);

    // Browse all the images 
    for (int i = 0; i < nbImageTaken; i++) {
        img = new Image();

        std::getline(*outputFile, line);
        splitStr(line, ' ', elems);

        img->la = std::stoi(elems[0]);
        img->lo = std::stoi(elems[1]);

        imgs.push_back(*img);
        elems.clear();
    }
    outputFile->clear();
    outputFile->seekg(0, std::ios::beg);
    return imgs;
}

/*
 * Check if no satellite moves the camera faster than w arcseconds per turn between taking two consecutive pictures.
 * Return true if, false otherwise
*/
bool JudgeOutput::isValidCamera() {
    Image * img1;
    Image * img2;
    int turn1, turn2;    
    Satelite * sat = new Satelite();
    std::string line;

    // Get the array satelite and number of it
    Satelite * arraySat = this->simData->getArraySat();
    long nbSatelite = this->simData->getNbSatelite();

     // Get the first line : number of image taken
	std::getline(*outputFile, line);
    std::vector<std::string> elems;
	int nbImageTaken = std::stoi(line);

    std::getline(*outputFile, line);
    img1 = new Image();
    splitStr(line, ' ', elems);

    img1->la = std::stoi(elems[0]);
    img1->lo = std::stoi(elems[1]);
    turn1 = std::stoi(elems[2]);

    sat = getSat(arraySat, nbSatelite, std::stoi(elems[3]));

    for (int i = 0; i < nbImageTaken - 1; i++) {
        img2 = new Image();
        
        std::getline(*outputFile, line);
        elems.clear();

        splitStr(line, ' ', elems);

        img2->la = std::stoi(elems[0]);
        img2->lo = std::stoi(elems[1]);
        turn2 = std::stoi(elems[2]);
        
        std::cout << img1->la << " " << img1->lo << " at turn " << turn1 << " with " << img2->la << " " << img2->lo << " at turn " << turn2 << std::endl;

        // Same satelite : we check camera
        if (sat->id == std::stoi(elems[3])) {
            if (!this->checkCamera(img1, img2, turn1, turn2, sat)) {
                return false;
            }
        }
        // Different satelite
        else {
            // Get the satelite from arraySat with his ID
            sat = getSat(arraySat, nbSatelite, std::stoi(elems[3]));
        }

        img1->la = img2->la;
        img1->lo = img2->lo;
        turn1 = turn2;

        elems.clear();
    }
    this->goBackToBeginningOutput();
    return true;
}

bool JudgeOutput::checkCamera(Image * lastPos, Image * img, int turn1, int turn2, Satelite * sat) {
    
    Satelite * satT1 = getSatPosition(sat, turn1);
    Satelite * satT2 = getSatPosition(sat, turn2);

    int lat1 = std::abs(satT1->la - lastPos->la);
    int lo1 = std::abs(satT1->lo - lastPos->lo);
    
    int lat2 = std::abs(satT2->la - lastPos->lo);
    int lo2 = std::abs(satT2->la - img->lo);

    int diffTurn = turn2 - turn1;

    return ( ((lat1 + lat2 <= satT1->speedRot) && ((lo1 + lo2) <= (diffTurn * satT1->speedRot))) );
}

/*
 * Get the score of the simulation 
 * Return the score
*/
int JudgeOutput::getScore() {
    int score = 0;
    
    int numCol;
    int numImg;
    bool colComplete;
    Image img;

    std::vector<Image> images = this->getImagesTaken();
    Collection * arrayCol = this->simData->getArrayCol();
    long nbCol = this->simData->getNbCollection();

    //  Browse the collection array 
    for (numCol = 0; numCol < nbCol; numCol++) { 
        Collection col = arrayCol[numCol];
        colComplete = true;
      //  std::cout << "Nb img : " << col.nbImg << std::endl;
        // Browse pictures of collection col
        for (numImg = 0; numImg < col.nbImg; numImg++) {
            img = col.listImg[numImg];
           // std::cout << "Col " << numCol << ", img " << numImg << " : " << img.la << " " << img.lo;
           
            // Check if img was taken
            if (!contains(images, img)) {
                colComplete = false;
                //std::cout << std::endl;
            }
            else  {
               // std::cout << " _/" << std::endl;
            }
        }
        // If col complete : add the score of col
        if (colComplete) {
            score += col.nbPts;
            //std::cout << "Collection " << numCol << " complete" << std::endl;
        }
    }
    return score;
}
