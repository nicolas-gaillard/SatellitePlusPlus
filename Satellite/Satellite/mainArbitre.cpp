#include "TimeMeasure.h"

/*
 * 2 paramètres :
 *    - répertoire des exécutable
 *    - nom du fichier de sortie
 *    - nom du jeu de données
 */
int main(int argc, char *argv[]) {
    TimeMeasure tm(argv[0]);

    if (argc == 1) {
        tm.setInputData("text/forever_alone.in");
        tm.setOutputResults("results.csv");
    }

    if (argc == 2){
        tm.setOutputResults(argv[1]);
        tm.setInputData("text/forever_alone.in");
    }

    if (argc == 3) {
        tm.setOutputResults(argv[1]);
        tm.setInputData(argv[2]);
    }

    std::vector<std::string> listFiles;
    tm.executeFolder();    
}