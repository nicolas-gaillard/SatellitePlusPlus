#include "TimeMeasure.h"

/*
 * 3 parameters possible (the first one is obligatory):
 *    - executable directory
 *    - output name
 *    - input name 
 */
int main(int argc, char *argv[]) {
    std::cout << argv[1] << std::endl;

    TimeMeasure tm(argv[1]);

    if (argc == 2) {
        tm.setInputData("../text/forever_alone.in");
        tm.setOutputResults("results.csv");
    }

    if (argc == 3){
        tm.setOutputResults(argv[2]);
        tm.setInputData("../text/forever_alone.in");
    }

    if (argc == 4) {
        tm.setOutputResults(argv[2]);
        tm.setInputData(argv[3]);
    }

    if (argc > 4) return 0;

    tm.executeFolder();    
}