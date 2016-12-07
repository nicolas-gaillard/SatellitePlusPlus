#include "TimeMeasure.h"

// Code based on http://stackoverflow.com/questions/306533/how-do-i-get-a-list-of-files-in-a-directory-in-c 
/*
 *
*/
void GetFilesInDirectory(std::vector<std::string> &out, const std::string &directory)
{
#ifdef WINDOWS
    HANDLE dir;
    WIN32_FIND_DATA file_data;

    if ((dir = FindFirstFile((directory + "/*").c_str(), &file_data)) == INVALID_HANDLE_VALUE)
        return; /* No files found */

    do {
        const std::string file_name = file_data.cFileName;
        const std::string full_file_name = directory + "/" + file_name;
        const bool is_directory = (file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;

        if (file_name[0] == '.')
            continue;

        if (is_directory)
            continue;

        out.push_back(full_file_name);
    } while (FindNextFile(dir, &file_data));

    FindClose(dir);
#else
    DIR *dir;
    struct dirent *ent;
    struct stat st;

    dir = opendir(directory.c_str());
    while ((ent = readdir(dir)) != NULL) {
        const std::string file_name = ent->d_name;
        const std::string full_file_name = directory + "/" + file_name;

        
        if (file_name[0] == '.')
            continue;

        if (stat(full_file_name.c_str(), &st) == -1)
            continue;

        const bool is_directory = (st.st_mode & S_IFDIR) != 0;

        if (is_directory)
            continue;
        
        out.push_back(full_file_name);
    }
    closedir(dir);
#endif
} 

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

    return static_cast<long>(execTime);
}

bool TimeMeasure::executeFolder(){

    return true; 
}