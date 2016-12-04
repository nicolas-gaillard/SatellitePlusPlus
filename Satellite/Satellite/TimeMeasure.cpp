#include "TimeMeasure.h"

TimeMeasure::TimeMeasure(std::string iFolder, std::string output, std::string iData){
    inputFolder = iFolder;
    nameOutput = new std::ifstream(output);
    inputData = iData;
}

// Launch executable and measure its execution time 
// Parameters : path of the resolution program and path of the output file
// Output : execution time in milliseconds
long TimeMeasure::measureExec(std::string pathExecutable, std::string outputExec){
    auto start = std::chrono::high_resolution_clock::now();

    // Launch the solution :
    // 1 executable takes two parameters : data set and output solution path file
    std::string commandTemp = pathExecutable + " \"" + inputData +  \
                                "\"" +  " \"" + outputExec + "\"";
    
    std::system(commandTemp.c_str());

    // Function to pause the system : 
    /*
    std::system("PAUSE");
    // Get a number between 1 and 60 (seconds) :
    int timeBreak = rand() % 60 + 1;
    sleep(timeBreak);
    */

    auto stop = std::chrono::high_resolution_clock::now();
	auto execTime = std::chrono::duration_cast<std::chrono::milliseconds>(stop-start).count();
    
    std::cout << "Solution  \"" + pathExecutable + \
                "\" with data set  \"" + inputData + "\" : " \
              << execTime \
              << " milliseconds" << std::endl;

    return (long)execTime;
}