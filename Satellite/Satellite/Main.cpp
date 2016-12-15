#include "JudgeOutput.h"

int main() {
    DataReceiver rc =  DataReceiver("text/forever_alone.in");
    SimulationData data = rc.extractData();
    JudgeOutput jo("text/forever_alone.out", &data);
    if (jo.isValidOutput()) {
        std::cout << "Score final : " << jo.getScore() << std::endl;
    }
    else {
        std::cout << "[E] Invalid ouput" << std::endl;
    }
}