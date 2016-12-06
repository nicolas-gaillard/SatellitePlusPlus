#include "JudgeOutput.h"
#include "DataReceiver.h"
#include <iostream>

int main() {
    DataReceiver rc =  DataReceiver("text/overlap.in");
	SimulationData data = rc.extractData();
    JudgeOutput jo("text/example.out");
    if (jo.isValidOutput()) {
        std::cout << "lolll" << std::endl;
        std::cout << jo.getScore(data.getArrayCol()) << std::cout;
    }
}