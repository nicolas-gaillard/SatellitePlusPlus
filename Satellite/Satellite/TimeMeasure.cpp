#include "TimeMeasure.h"

TimeMeasure::~TimeMeasure(){}

TimeMeasure::TimeMeasure(std::string iFolder, std::string output, std::string iData){
    inputFolder = iFolder;
    outputResults = new std::ifstream(output);
    inputData = iData;
}

/*
 * Launch executable and measure its execution time 
 * Parameters : path of the resolution program and path of the output file
 * Output : execution time in milliseconds
*/
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

    return static_cast<long>(execTime);
}

// Code based on http://stackoverflow.com/questions/306533/how-do-i-get-a-list-of-files-in-a-directory-in-c 
/*
 * List all the files in a directory.
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
 * Launch each executable in the folder
 * Parameters : 
 * Output : boolean 
*/
bool TimeMeasure::executeFolder(){
    std::vector<std::string> execList;

    if (getFilesInDirectory(execList)) {
        for (auto &vit : execList){
            measureExec(vit, vit + "_out");
        }
    }

    return true; 
}

bool TimeMeasure::createResults(){
    return true;
}