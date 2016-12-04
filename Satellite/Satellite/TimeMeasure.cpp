#include "TimeMeasure.h"

TimeMeasure::TimeMeasure(std::string iFolder, std::string output, std::string iData){
    inputFolder = iFolder;
    nameOutput = new std::ifstream(output);
    inputData = iData;
}

// Launch executable and measure its execution time 
long TimeMeasure::measureExec(std::string pathExecutable, std::string outputExec){
    // Launch the solution :
    /*
    // 1 executable takes two parameters : data set and output solution path file
    std::string commandTemp = pathExecutable + " \"" + inputData +  \
                            "\"" +  " \"" + outputExec + "\"";
    
    std::system(commandTemp.c_str());
    */ 

    // Function to pause the system : 
    // std::system("PAUSE");

    // Get a number between 1 and 30 :
    int timeBreak = rand() % 30 + 1;
    sleep(timeBreak);

    return NULL;
}