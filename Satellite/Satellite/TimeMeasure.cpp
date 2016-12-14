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
 * Output : execution time in milliseconds
 */
long TimeMeasure::measureExec(std::string pathExecutable, std::string outputExec){

    /*
     * TODO :
     *   - Changer l'ordre de grandeur des mesures 
     */

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
            // Adding the execution time (and leaving the score at 0) 
            resultTabs[vit] = std::make_pair(measureExec(vit, vit + "_out"), 0);
        }
    }
    
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
        
        for (; mit != resultTabs.end(); mit++){
            outputResults << "Exécutable : " << mit->first << std::endl;
            outputResults << "Temps d'éxécution : " << mit->second.first << " ms" << std::endl;
            outputResults << "Score : " << mit->second.second << " points" << std::endl << std::endl;
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
                      << " (" << minTime.second << " ms)";
                      // time unit will change

        // Best ratio :
        outputResults << std::endl << std::endl
                      << "Meilleure solution globale (meilleur ratio) : "
                      << bestCompromise.first
                      << " (" << bestCompromise.second << ")";

        
        
        // Version CSV :
        outputResults << "Tableaux de résultats \n \n" ;
        outputResults << "Exec, Time, Score \n";
        
        auto it = resultTabs.begin();

        for (; it != resultTabs.end(); it++){
            outputResults << it->first << "," 
                          << it->second.first << ","
                          << it->second.second << " \n";
        }


        // We don't have to close it, destructor does this job. 
        // outputResults.close();

        return true;
    }

    return false;
}