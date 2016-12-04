#include "TimeMeasure.h"

TimeMeasure::TimeMeasure(std::string iFolder, std::string output, std::string iData){
    inputFolder = iFolder;
    nameOutput = new std::ifstream(output);
    inputData = iData;
}


long TimeMeasure::measureExec(std::string pathExecutable, std::string outputExec){
    // 1 executable takes two parameters : data set and output solution path file
    std::string commandTemp = pathExecutable + " \"" + inputData +  \
                            "\"" +  " \"" + outputExec + "\"";
    
    std::system(commandTemp.c_str());
    return NULL;
}