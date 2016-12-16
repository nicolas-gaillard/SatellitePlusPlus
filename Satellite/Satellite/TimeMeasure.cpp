#include "TimeMeasure.h"

TimeMeasure::~TimeMeasure(){}

/*
 * TODO :
 *  - Voir si les valeurs par défaut sont bonnes (préciser le dossier ?)
 */

TimeMeasure::TimeMeasure(std::string iFolder){
    inputFolder = iFolder;
    inputData = "forever_alone.in";
    outputResults = std::fstream("results.csv", std::fstream::in | std::fstream::out | std::fstream::trunc);
}

TimeMeasure::TimeMeasure(std::string iFolder, std::string output, std::string iData){    
    inputFolder = iFolder;
    outputResults = std::fstream(output.c_str(), std::fstream::in | std::fstream::out | std::fstream::trunc);
    inputData = iData;
}

/*
 * Launches one executable and measures its execution time 
 * Parameters : path of the resolution program and path of the output file
 * Output : execution time in seconds
 */
long TimeMeasure::measureExec(std::string pathExecutable, std::string outputExec){

    // Launch the solution :
    // 1 executable takes two parameters : data set and output solution path file
    std::string commandTemp = pathExecutable + " \"" + inputData +  \
                                "\"" +  " \"" + outputExec + "\"";

    auto start = std::chrono::steady_clock::now();
    
    std::cout << "Launching the solution : " << pathExecutable << std::endl;

    std::system(commandTemp.c_str());

    // Function to pause the system : 
    /*
    std::system("PAUSE");
    // Get a number between 1 and 60 (seconds) :
    int timeBreak = rand() % 60 + 1;
    sleep(timeBreak);
    */

    auto stop = std::chrono::steady_clock::now();
	auto execTime = std::chrono::duration_cast<std::chrono::seconds>(stop-start).count();
    
    std::cout << "Solution  \"" + pathExecutable + \
                "\" with data set  \"" + inputData + "\" : " \
              << execTime \
              << " seconds" << std::endl;

    return static_cast<long>(execTime);
}

long TimeMeasure::measureExec(std::string pathExecutable){


    std::string pathOut = pathExecutable + "_" + (inputData.substr(0, inputData.size() - 2)) + "out";
    
    // (pathExecutable.substr(0, pathExecutable.size() - 2)).append("out");
    
    std::string commandTemp = pathExecutable + " \"" + inputData +  \
                                "\"" +  " \"" + pathOut + "\"";

    auto start = std::chrono::steady_clock::now();

    std::system(commandTemp.c_str());

    auto stop = std::chrono::steady_clock::now();
	auto execTime = std::chrono::duration_cast<std::chrono::seconds>(stop-start).count();
    
    return static_cast<long>(execTime);
}

// Code based on http://stackoverflow.com/questions/306533/how-do-i-get-a-list-of-files-in-a-directory-in-c 
/*
 * Lists all the files in a directory.
 * Parameters : vector which will store all the file, name of the directory
 * Output : nothing
 */
bool TimeMeasure::getFilesInDirectory(std::vector<std::string> &out)
{
#ifdef WINDOWS
    HANDLE dir;
    WIN32_FIND_DATA file_data;

    if ((dir = FindFirstFile((inputFolder + "/*").c_str(), &file_data)) == INVALID_HANDLE_VALUE)
        // No files found 
        return false; 

    do {
        // Get the file's name
        const std::string file_name = file_data.cFileName;

        // Get the file's path
        const std::string full_file_name = inputFolder + "/" + file_name;

        // Check if the file is a directory :
        const bool is_directory = (file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;

        // If the file is not a directory, add it on the vector 
        if (!((file_name[0] == '.') || (is_directory)))
            out.push_back(full_file_name);

    } while (FindNextFile(dir, &file_data));
    // While there are files in the folder 

    FindClose(dir);
    return true;

#else
    DIR *dir;
    struct dirent *ent;
    struct stat st;

    dir = opendir(inputFolder.c_str());
    // While there are files in the folder :
    while ((ent = readdir(dir)) != NULL) {
        // Get the file's name :
        const std::string file_name = ent->d_name;

        // Get the file's path :
        const std::string full_file_name = inputFolder + "/" + file_name;

        // Check if the file is a directory :
        const bool is_directory = (st.st_mode & S_IFDIR) != 0;

        //std::cout << full_file_name << std::endl;

        // If the file is not a directory, add it on the vector 
        if (!((file_name[0] == '.') || (stat(full_file_name.c_str(), &st) == -1) || is_directory))
            out.push_back(full_file_name);
    }

    if (closedir(dir) == 0) 
        return true;
    else {
        std::cerr << "Can't close the directory" ;
        return false;
    }
    
#endif
} 

bool has_suffix(const std::string &str, const std::string &suffix)
{
    return str.size() >= suffix.size() &&
           str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

/*
 * Launches each executable in the folder
 * Parameters : 
 * Output : boolean 
 */
bool TimeMeasure::executeFolder(){
    // It contains the path of each executable
    std::vector<std::string> execList;

    // Initialization of the vector :
    if (getFilesInDirectory(execList)) {
        // Browsing the vector :
        for (auto &vit : execList){

            std::cout << "vecteur : " << vit  << std::endl;
            
            std::string pathOutputExec;
            pathOutputExec = vit + "_" + (inputData.substr(0, inputData.size() - 2)) + "out";

            // std::cout << vit << std::endl;

            // Adding the execution time (and leaving the score at 0)
            if ( !(has_suffix(vit, "out")) || !(has_suffix(vit, "in"))) resultTabs[vit] = std::make_pair(measureExec(vit, pathOutputExec), 0);

            // Check the output file :
            DataReceiver rc = DataReceiver(inputData,15,1);
            SimulationData data = rc.extractData();
            JudgeOutput jo(pathOutputExec, &data); 

            // If the output is valid, we update the score 
            if (jo.isValidOutput()) {
                resultTabs[vit].second = static_cast<long>(jo.getScore());
            }
        }
    }

    createResults();
    
    /*
     * To modify only the second value of the pair :
     * myPair.second = newValue;
     */

    return true; 
}

/*
 * Writes in an output file the performance of each executable
 * Parameters : nothing
 * Output : bool
 */
bool TimeMeasure::createResults(){
    // Normally, the file has been opened during the initialization
    if (outputResults.is_open()) {
        
        auto mit = resultTabs.begin();

         // Ratio : score per time;
        long ratio = static_cast<long>(mit->second.second) / mit->second.first;
        std::pair<std::string, long> bestCompromise(mit->first, ratio);
        std::pair<std::string, int> maxScore(mit->first, mit->second.second);
        std::pair<std::string, long> minTime(mit->first, mit->second.first);
        
        // TXT mode :
        for (; mit != resultTabs.end(); mit++){
            outputResults << "Exécutable : " << mit->first << std::endl;
            outputResults << "Temps d'éxécution : " << mit->second.first << " seconds" << std::endl;
            outputResults << "Score : " << mit->second.second << " points" << std::endl << std::endl;
            
            if (mit->second.first < minTime.second) {
                minTime.first = mit->first;
                minTime.second = mit->second.first;
            }

            if (mit->second.second > maxScore.second) {
                maxScore.first = mit->first;
                maxScore.second = mit->second.second;
            }

            long tmpRatio = static_cast<long>(mit->second.second) / mit->second.first;
            if (tmpRatio > bestCompromise.second) {
                bestCompromise.first = mit->first;
                bestCompromise.second = tmpRatio;
            }
            
        }
        
        // Best score :
        outputResults << std::endl << std::endl 
                      << "Meilleure solution en terme de score : " 
                      << maxScore.first
                      << " (" << maxScore.second << " points)";

        // Best time :
        outputResults << std::endl << std::endl 
                      << "Meilleure solution en terme de temps : "
                      << minTime.first
                      << " (" << minTime.second << " seconds)";
                      // time unit will change

        // Best ratio :
        outputResults << std::endl << std::endl
                      << "Meilleure solution globale (meilleur ratio) : "
                      << bestCompromise.first
                      << " (" << bestCompromise.second << ")";

        
        // CSV mode :
        /*
        outputResults << "Tableaux de résultats \n \n" ;
        outputResults << "Exec, Time (s), Score \n";

        for (; mit != resultTabs.end(); mit++){
            outputResults << mit->first << "," 
                          << mit->second.first << ","
                          << mit->second.second << "\n";
            
            if (mit->second.first < minTime.second) {
                minTime.first = mit->first;
                minTime.second = mit->second.first;
            }

            if (mit->second.second > maxScore.second) {
                maxScore.first = mit->first;
                maxScore.second = mit->second.second;
            }

            long tmpRatio = static_cast<long>(mit->second.second) / mit->second.first;
            if (tmpRatio > bestCompromise.second) {
                bestCompromise.first = mit->first;
                bestCompromise.second = tmpRatio;
            }
        }

        outputResults << "\n \n";
        outputResults << "Best score :," << maxScore.first << "," << maxScore.second << "\n";
        outputResults << "Best time :," << minTime.first << "," << minTime.second << "\n";
        outputResults << "Best compromise:," << bestCompromise.first << "," << bestCompromise.second << "\n";

        // We don't have to close it, destructor does this job. 
        // outputResults.close();
        */
        return true;
    }

    return false;
}